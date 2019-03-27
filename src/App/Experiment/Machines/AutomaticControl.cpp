#include "AutomaticControl.h"

// Include extenral deps
#include <QObject>


namespace App { namespace Experiment { namespace Machines
{
    AutomaticControl::AutomaticControl(QObject *parent, Settings::Container* settings, Hardware::Access& hardware,
                                       ReadGuageTrip& trips, PumpControl& pumpController)
        :   Helpers::MachineStates(parent, settings, hardware)

            // Settings container
        ,   m_settings(settings)

            // Timers
        ,   t_pumpManifold(*new QTimer(parent))
        ,   t_pumpBarrel(*new QTimer(parent))

            // state machines
        ,   m_pumpController(pumpController)
        ,   m_manifoldLeakDetection(*new LeakDetection(parent, settings, hardware))
        ,   m_barrelLeakDetection(*new LeakDetection(parent, settings, hardware))
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        t_pumpBarrel.setInterval(10000);

        // Record when trips are triggered
        connect(&m_hardware, &Hardware::Access::emit_guageReadVacuum, this, &AutomaticControl::guagePressures);
        connect(&trips, &ReadGuageTrip::emit_guageTripped, this, &AutomaticControl::guageTripped);

        // State machine connections
        connect(state("selectPump", true), &QState::entered, this, &AutomaticControl::selectPump);

        connect(state("requestPump1", true), &QState::entered, this, &AutomaticControl::requestPump1);
        connect(state("requestPump2", true), &QState::entered, this, &AutomaticControl::requestPump2);

        connect(state("markPumpFailure", true), &QState::entered, this, &AutomaticControl::markPumpFailure);

        connect(state("selectBarrel", true), &QState::entered, this, &AutomaticControl::selectBarrel);
        connect(state("noBarrelsWait", true), &QState::entered, this, &AutomaticControl::manifoldLeakDetection);

        connect(state("manifoldPressure", true), &QState::entered, this, &AutomaticControl::manifoldPressure);
        connect(state("manifoldPumpTimer", true), &QState::entered, this, &AutomaticControl::isRunningManifoldPumpTimer);
        connect(state("manifoldLeakDetect", true), &QState::entered, this, &AutomaticControl::manifoldLeakDetection);

        connect(state("closePumpValve", true), &QState::entered, this, &AutomaticControl::closePumpValve);
        connect(validator("closePumpValve", true), &QState::entered, this, &AutomaticControl::validateClosePumpValve);

        connect(state("openBarrel", true), &QState::entered, this, &AutomaticControl::openBarrelValve);
        connect(validator("openBarrel", true), &QState::entered, this, &AutomaticControl::validateOpenBarrelValve);

        connect(state("barrelLeakDetect", true), &QState::entered, this, &AutomaticControl::startBarrelLeakDetect);

        connect(state("failureCloseBarrel", true), &QState::entered, this, &AutomaticControl::closeBarrelValve);
        connect(state("failureMarkBarrel", true), &QState::entered, this, &AutomaticControl::failureMarkBarrel);
        connect(state("failureOpenPumpValve", true), &QState::entered, this, &AutomaticControl::failureOpenPumpValve);

    }

    AutomaticControl::~AutomaticControl()
    {

    }


    /**
     * Set the commands to be used by the machine
     *
     */
    void AutomaticControl::setParams()
    {       
        // Set enabled valves
        for(int i : {1,2,3,4,5,6})
        {
            if(m_settings->general()->chamber(i)["auto_control_enabled"].toBool())
                m_enabled.append(i);
        }

        // Set pump states
        m_disabledPump1 = (m_settings->general()->pump(1)["auto_control_enabled"].toBool()) ? 0 : 1;
        m_disabledPump2 = (m_settings->general()->pump(2)["auto_control_enabled"].toBool()) ? 0 : 1;
    }


    /**
     * Start the state machine
     *
     * @brief AutomaticControl::start
     */
    void AutomaticControl::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief AutomaticControl::stopped
     */
    void AutomaticControl::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief AutomaticControl::buildMachine
     */
    void AutomaticControl::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("selectPump", true));

        // Which pump should be turn on
        state("selectPump", true)->addTransition(this, &AutomaticControl::emit_usePump1, state("requestPump1", true));
        state("selectPump", true)->addTransition(this, &AutomaticControl::emit_usePump2, state("requestPump2", true));
        state("selectPump", true)->addTransition(this, &AutomaticControl::emit_noPumpsAvailable, &sm_stopAsFailed);

        state("requestPump1", true)->addTransition(&m_pumpController, &PumpControl::emit_machineFinished, state("selectBarrel", true));
        state("requestPump1", true)->addTransition(&m_pumpController, &PumpControl::emit_machineFailed, state("markPumpFailure", true));

        state("requestPump2", true)->addTransition(&m_pumpController, &PumpControl::emit_machineFinished, state("selectBarrel", true));
        state("requestPump2", true)->addTransition(&m_pumpController, &PumpControl::emit_machineFailed, state("markPumpFailure", true));

        // Mark as failed pump AND @TODO shutdown pump 2
        state("markPumpFailure", true)->addTransition(this, &AutomaticControl::emit_pumpMarkedAsFailed, state("selectPump", true));


        // Select barrel
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_1, state("manifoldPressure", true));
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_2, state("manifoldPressure", true));
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_3, state("manifoldPressure", true));
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_4, state("manifoldPressure", true));
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_5, state("manifoldPressure", true));
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_6, state("manifoldPressure", true));
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_noBarrelAvailable, state("noBarrelsWait", true));

            // Manifold leak detect
            state("noBarrelsWait", true)->addTransition(&m_manifoldLeakDetection, &LeakDetection::emit_machineFailed, state("selectPump", true));
            state("noBarrelsWait", true)->addTransition(&m_manifoldLeakDetection, &LeakDetection::emit_machineFinished, state("selectBarrel", true));

        // Check manifold pressure
        state("manifoldPressure", true)->addTransition(this, &AutomaticControl::emit_possiablePumpManifoldLeak, state("selectPump", true));
        state("manifoldPressure", true)->addTransition(this, &AutomaticControl::emit_vacuumManifoldNotSufficent, state("manifoldPumpTimer", true));  // state("manifoldPumpTimer", true)
        state("manifoldPressure", true)->addTransition(this, &AutomaticControl::emit_vacuumManifoldSufficent, state("closePumpValve", true));

            // How long have we been pumping
            state("manifoldPumpTimer", true)->addTransition(this, &AutomaticControl::emit_totalTimeOk, state("manifoldLeakDetect", true));
            state("manifoldPumpTimer", true)->addTransition(this, &AutomaticControl::emit_totalTimeExceeded, state("selectPump", true));

            state("manifoldLeakDetect", true)->addTransition(&m_manifoldLeakDetection, &LeakDetection::emit_machineFailed, state("selectPump", true));
            state("manifoldLeakDetect", true)->addTransition(&m_manifoldLeakDetection, &LeakDetection::emit_machineFinished, state("manifoldPressure", true));

        // Check barrel
        state("closePumpValve", true)->addTransition(this, &AutomaticControl::emit_invalidPumpNumber, &sm_stopAsFailed);
        state("closePumpValve", true)->addTransition(&m_hardware, &Hardware::Access::emit_valveClosed, validator("closePumpValve", true));
            validator("closePumpValve", true)->addTransition(this, &AutomaticControl::emit_validationWrongId, state("closePumpValve", true));
            validator("closePumpValve", true)->addTransition(this, &AutomaticControl::emit_validationSuccess, state("openBarrel", true));
            validator("closePumpValve", true)->addTransition(this, &AutomaticControl::emit_validationFailed, &sm_stopAsFailed);

        state("openBarrel", true)->addTransition(&m_hardware, &Hardware::Access::emit_valveOpened, validator("openBarrel", true));
            validator("openBarrel", true)->addTransition(this, &AutomaticControl::emit_validationWrongId, state("openBarrel", true));
            validator("openBarrel", true)->addTransition(this, &AutomaticControl::emit_validationSuccess, state("barrelLeakDetect", true));
            validator("openBarrel", true)->addTransition(this, &AutomaticControl::emit_validationFailed, &sm_stopAsFailed);

        state("barrelLeakDetect", true)->addTransition(&m_barrelLeakDetection, &LeakDetection::emit_machineFailed, state("failureCloseBarrel", true));
        state("barrelLeakDetect", true)->addTransition(&m_barrelLeakDetection, &LeakDetection::emit_machineFinished, state("openPumpValve", true));

            // Close barrel X valve @todo needs validation
            state("failureCloseBarrel", true)->addTransition(&m_hardware, &Hardware::Access::emit_valveClosed, state("failureMarkBarrel", true));
            state("failureMarkBarrel", true)->addTransition(this, &AutomaticControl::emit_barrelMarkedAsLeaked ,state("failureOpenPumpValve", true));
            state("failureOpenPumpValve", true)->addTransition(&m_hardware, &Hardware::Access::emit_valveOpened, state("selectBarrel", true));


        // Open pump valve and start pumping


        // Do other barrels need pumping?


        // Close barrel valve return to select barrel

    }


    void AutomaticControl::closeBarrelValve()
    {
        qDebug() << "Closing barrel valve" << m_currentBarrel;

        switch(m_currentBarrel)
        {
            case 1:
                valves()->closeGroup1();
            break;
            case 2:
                valves()->closeGroup2();
            break;
            case 3:
                valves()->closeGroup3();
            break;
            case 4:
               valves()->closeGroup4();
            break;
            case 5:
                valves()->closeGroup5();
            break;
            case 6:
                valves()->closeGroup6();
            break;
            default:
                emit emit_invalidBarrelNumber();
                return;
        }
    }

    void AutomaticControl::failureMarkBarrel()
    {
        qDebug() << "Marking barrel as leaked" << m_currentBarrel;

        if(!m_leaked.contains(m_currentBarrel))
            m_leaked.append(m_currentBarrel);

        emit emit_barrelMarkedAsLeaked(m_currentBarrel);
    }

    void AutomaticControl::failureOpenPumpValve()
    {
        qDebug() << "Failuring opening pump valve" << (m_currentPump + 6);
        if((m_currentPump + 6) == 7)
        {
            valves()->openGroup7();
            return;
        }
        else if((m_currentPump + 6) == 8)
        {
            valves()->openGroup8();
            return;
        }
    }


    void AutomaticControl::startBarrelLeakDetect()
    {
        qDebug() << "Barrel leak detector on : "<<m_currentBarrel;
        auto barrel = m_settings->general()->chamber(m_currentBarrel);

        m_barrelLeakDetection.setParams(m_currentBarrel, barrel["leak_period"].toInt(), barrel["leak_max"].toInt(), 4, 2000);
        m_barrelLeakDetection.start();
    }


    void AutomaticControl::validateOpenBarrelValve()
    {
        qDebug() << "Validating valve" << m_currentBarrel;

        // Get the validator state instance
        Helpers::CommandValidatorState* command = dynamic_cast<Helpers::CommandValidatorState*>(sender());

        // If cast successfull
        if(command != nullptr && command->package.value("cast_status").toBool())
        {
            // Get the package data from the instance
            QVariantMap package = command->package;

            qDebug() << package;

            // Check valve is the same
            if(package.value("group").toInt() == m_currentBarrel && true == package.value("value").toBool())
            {
                // Emit safe to proceed
                emit emit_validationSuccess();
                return;
            }

            emit emit_validationWrongId();
            return;
        }

        // Emit not safe to proceed
        emit emit_validationFailed();
    }

    void AutomaticControl::openBarrelValve()
    {
        qDebug() << "Opening valve" << m_currentBarrel;
        switch(m_currentBarrel)
        {
            case 1:
                valves()->openGroup1();
            break;
            case 2:
                valves()->openGroup2();
            break;
            case 3:
                valves()->openGroup3();
            break;
            case 4:
               valves()->openGroup4();
            break;
            case 5:
                valves()->openGroup5();
            break;
            case 6:
                valves()->openGroup6();
            break;
            default:
                emit emit_invalidBarrelNumber();
                return;
        }
    }

    void AutomaticControl::validateClosePumpValve()
    {
        qDebug() << "Validating close pump valve" << (m_currentPump + 6);

        // Get the validator state instance
        Helpers::CommandValidatorState* command = dynamic_cast<Helpers::CommandValidatorState*>(sender());

        // If cast successfull
        if(command != nullptr && command->package.value("cast_status").toBool())
        {
            // Get the package data from the instance
            QVariantMap package = command->package;

            qDebug() << package;

            // Check valve is the same
            if(package.value("group").toInt() == (m_currentPump + 6) && false == package.value("value").toBool())
            {
                // Emit safe to proceed
                emit emit_validationSuccess();

                return;
            }

            emit emit_validationWrongId();
            return;
        }

        // Emit not safe to proceed
        emit emit_validationFailed();
    }

    void AutomaticControl::closePumpValve()
    {
        qDebug() << "Closing pump valve" << (m_currentPump + 6);
        if((m_currentPump + 6) == 7)
        {
            valves()->closeGroup7();
            return;
        }
        else if((m_currentPump + 6) == 8)
        {
            valves()->closeGroup8();
            return;
        }

        // return emit_invalidPumpNumber();
    }


    void AutomaticControl::isRunningManifoldPumpTimer()
    {
        qDebug() << "Manifold pumping timer" << (m_currentPump + 6);
        if(t_pumpManifold.isActive())
        {
            emit emit_totalTimeOk();
            return;
        }

        emit emit_totalTimeExceeded();
    }


    void AutomaticControl::manifoldLeakDetection()
    {
        qDebug() << "Check for manifold leak:" << (m_currentPump + 6);
        m_manifoldLeakDetection.setParams((m_currentPump + 6), 10000, 1, 10, 3000);
        m_manifoldLeakDetection.start();
    }


    void AutomaticControl::manifoldPressure()
    {
        // Current barrel reading
        auto currentBarrel = m_pressures[m_currentBarrel];

        if(!m_manifoldTimerStarted)
        {
            qDebug() << "Starting mani timer" << t_pumpManifold.interval();
            t_pumpManifold.setSingleShot(true);
            t_pumpManifold.start();
            m_manifoldTimerStarted = true;
        }

        // If less than barrel pressure we're good to go
        if(m_pressures[(m_currentPump + 6)] < currentBarrel)
        {
            m_manifoldTimerStarted = false;
            t_pumpManifold.stop();
            emit emit_vacuumManifoldSufficent();
            return;
        }

        // Check if the pressure change from previous is none (possiable small leak event)
        /*if(m_manifoldLastPressure > 0 && ((m_manifoldLastPressure - currentBarrel) > 0))
        {
            m_manifoldTimerStarted = false;
            t_pumpManifold.stop();
            emit emit_possiablePumpManifoldLeak();
        }*/

        // Mark last check
        m_manifoldLastPressure = currentBarrel;

        // Requires more pumping
        emit emit_vacuumManifoldNotSufficent();
    }


    void AutomaticControl::selectBarrel()
    {
        // Find max enabled barrel
        int maxBarrelId = 1; // Down to up
        for (auto i : m_enabled) {
            maxBarrelId = qMax(maxBarrelId, i);
        }

        // Find lowest enabled barrel
        int minBarrelId = 6; // Up to down
        for (auto i : m_enabled) {
            minBarrelId = qMin(minBarrelId, i);
        }

        // Increment current by one so we dont repeat it
        int startingPoint = (m_currentBarrel == maxBarrelId) ? minBarrelId : m_currentBarrel+1;

        // Find next barrel
        int foundBarrel = false;
        for(int a = startingPoint; a <= 6; ++a)
        {
            if(m_enabled.contains(a))
            {
                // Check barrel pressure above lower set point
                if(m_pressures[a] <= m_settings->general()->chamber(a)["lower_set_point"].toDouble())
                    continue;

                // Check barrel guage has not tripped
                if(m_tripped.contains(a))
                    continue;

                // Check barrel is not marked as leaked
                if(m_leaked.contains(a))
                    continue;

                // Check barrel is not marked as heavy load
                if(m_heavyLoad.contains(a))
                    continue;

                // We have found a barrel that we can work on next
                foundBarrel = true;

                // This barrel needs topping up
                m_currentBarrel = a;

                // Set pumping time for barrel
                t_pumpBarrel.setInterval(10000);

                // Reset the manifold last pressure
                m_manifoldLastPressure = -1;

                // Break from loop
                break;
            }
        }

        if(!foundBarrel)
        {
            qDebug() << "No barrel found";
            emit emit_noBarrelAvailable();
            return;
        }

        qDebug() << "Selecting barrel:" << m_currentBarrel;


        // Return the state to transition to
        switch(m_currentBarrel)
        {
            case 1:
                emit emit_goTo_1();
            break;
            case 2:
                emit emit_goTo_2();
            break;
            case 3:
                emit emit_goTo_3();
            break;
            case 4:
                emit emit_goTo_4();
            break;
            case 5:
                emit emit_goTo_5();
            break;
            case 6:
                emit emit_goTo_6();
            break;
        }
    }




    /**
     * Selects which pump should be used
     *
     * @brief AutomaticControl::selectPump
     */
    void AutomaticControl::selectPump()
    {
        // Switch current pump
        int selectedPump = (m_currentPump == 1) ? 2 : 1;

        // Make sure pump allowed
        if(selectedPump == 1 && m_disabledPump1 == 0)
        {
            // Mark pump 1 as on
            m_currentPump = 1;

            // Emit pump 1 to be used
            emit emit_usePump1();

            // Set manifold timer
            t_pumpManifold.setInterval((m_settings->general()->pump(1)["pump_manifold_void"].toInt() * 60) * 1000 );

            // No more work here
            return;
        }
        else if(selectedPump == 2 && m_disabledPump2 == 0)
        {
            // Mark pump 2 as on
            m_currentPump = 2;

            // Emit pump 2 to be used
            emit emit_usePump2();

            // Set manifold timer
            t_pumpManifold.setInterval((m_settings->general()->pump(2)["pump_manifold_void"].toInt() * 60) * 1000 );

            // No more work here
            return;
        }

        // No pumps avaliable
        emit emit_noPumpsAvailable();
    }


    /**
     * Request pump 1 to be turned on
     *
     * @brief AutomaticControl::requestPump1
     */
    void AutomaticControl::requestPump1()
    {
        m_pumpController.setParams(1);
        m_pumpController.start();
    }


    /**
     * Request pump 2 to be turned on
     *
     * @brief AutomaticControl::requestPump2
     */
    void AutomaticControl::requestPump2()
    {
        m_pumpController.setParams(2);
        m_pumpController.start();
    }



    void AutomaticControl::markPumpFailure()
    {
        // Mark pump as leak detected
        if(m_currentPump == 1)
        {
            m_disabledPump1 = 2;
            emit emit_pumpMarkedAsFailed(1);
        }
        else if(m_currentPump == 2)
        {
            m_disabledPump2 = 2;
            emit emit_pumpMarkedAsFailed(1);
        }
    }




    /**
     * Keep track of tripped valves
     *
     * @brief AutomaticControl::guageTripped
     * @param group
     * @param state
     */
    void AutomaticControl::guageTripped(int group, bool state)
    {
        int i = m_tripped.indexOf(group);

        if(i == -1)
        {
            if(state)
                m_tripped.append(group);
        }
        else
        {
            if(!state)
                m_tripped.removeAt(i);
        }
    }



    /**
     * Keep track of pressures
     *
     * @brief AutomaticControl::guagePressures
     * @param package
     */
    void AutomaticControl::guagePressures(QVariantMap package)
    {
        int group = package.value("guage_id").toInt();
        double value = package.value("pressure_mbar").toDouble();

        m_pressures.insert(group, value);
    }



}}}






