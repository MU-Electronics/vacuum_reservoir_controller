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
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Record when trips are triggered
        connect(&trips, &ReadGuageTrip::emit_guageTripped, this, &AutomaticControl::guageTripped);

        // State machine connections
        connect(state("selectPump", true), &QState::entered, this, &AutomaticControl::selectPump);

        connect(state("requestPump1", true), &QState::entered, this, &AutomaticControl::requestPump1);
        connect(state("requestPump2", true), &QState::entered, this, &AutomaticControl::requestPump2);

        connect(state("markPumpFailure", true), &QState::entered, this, &AutomaticControl::markPumpFailure);

        connect(state("selectBarrel", true), &QState::entered, this, &AutomaticControl::selectBarrel);

        connect(state("manifoldPressure", true), &QState::entered, this, &AutomaticControl::manifoldPressure);
        connect(state("manifoldPumpTimer", true), &QState::entered, this, &AutomaticControl::startManifoldPumpTimer);
        connect(state("manifoldLeakDetect", true), &QState::entered, this, &AutomaticControl::manifoldLeakDetection);

        connect(state("closePumpValve", true), &QState::entered, this, &AutomaticControl::closePumpValve);
        connect(state("closePumpValve", true), &QState::entered, this, &AutomaticControl::validateClosePumpValve);

        connect(state("openBarrel", true), &QState::entered, this, &AutomaticControl::openBarrelValve);
        connect(validator("openBarrel", true), &QState::entered, this, &AutomaticControl::validateOpenBarrelValve);

        connect(state("pumpingBarrelTimer", true), &QState::entered, this, &AutomaticControl::startBarrelPumpTimer);

        connect(state("barrelLeakDetect", true), &QState::entered, this, &AutomaticControl::startBarrelLeakDetect);


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

        state("markPumpFailure", true)->addTransition(this, &AutomaticControl::emit_pumpMarkedAsFailed, state("selectPump", true));


        // Select barrel
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_1, state("manifoldPressure", true));
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_2, state("manifoldPressure", true));
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_3, state("manifoldPressure", true));
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_4, state("manifoldPressure", true));
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_5, state("manifoldPressure", true));
        state("selectBarrel", true)->addTransition(this, &AutomaticControl::emit_goTo_6, state("manifoldPressure", true));

        // Check manifold pressure
        state("manifoldPressure", true)->addTransition(this, &AutomaticControl::emit_possiablePumpManifoldLeak, state("selectPump", true));
        state("manifoldPressure", true)->addTransition(this, &AutomaticControl::emit_vacuumManifoldNotSufficent, state("manifoldPumpTimer", true));
        state("manifoldPressure", true)->addTransition(this, &AutomaticControl::emit_vacuumManifoldSufficent, state("manifoldPumpTimer", true));

            state("manifoldPumpTimer", true)->addTransition(&t_pumpManifold, &QTimer::timeout, state("manifoldLeakDetect", true));

            state("manifoldLeakDetect", true)->addTransition(&m_manifoldLeakDetection, &LeakDetection::emit_machineFailed, state("manifoldPressure", true));
            state("manifoldLeakDetect", true)->addTransition(&m_manifoldLeakDetection, &LeakDetection::emit_machineFinished, state("closePumpValve", true));

        state("closePumpValve", true)->addTransition(this, &AutomaticControl::emit_invalidPumpNumber, &sm_stopAsFailed);
        state("closePumpValve", true)->addTransition(&m_hardware, &Hardware::Access::emit_valveClosed, validator("closePumpValve", true));
            validator("closePumpValve", true)->addTransition(this->valves(), &Functions::ValveFunctions::emit_validationWrongId, state("closePumpValve", true));
            validator("closePumpValve", true)->addTransition(this->valves(), &Functions::ValveFunctions::emit_validationSuccess, state("openBarrel", true));
            validator("closePumpValve", true)->addTransition(this->valves(), &Functions::ValveFunctions::emit_validationFailed, &sm_stopAsFailed);

        state("openBarrel", true)->addTransition(&m_hardware, &Hardware::Access::emit_valveOpened, validator("openBarrel", true));
            validator("openBarrel", true)->addTransition(this->valves(), &Functions::ValveFunctions::emit_validationWrongId, state("openBarrel", true));
            validator("openBarrel", true)->addTransition(this->valves(), &Functions::ValveFunctions::emit_validationSuccess, state("pumpingBarrelTimer", true));
            validator("openBarrel", true)->addTransition(this->valves(), &Functions::ValveFunctions::emit_validationFailed, &sm_stopAsFailed);

        state("pumpingBarrelTimer", true)->addTransition(&t_pumpBarrel, &QTimer::timeout, state("barrelLeakDetect", true));

    }


    void AutomaticControl::startBarrelLeakDetect()
    {
        qDebug() << "Barrel leak detect";
    }


    void AutomaticControl::startBarrelPumpTimer()
    {
        qDebug() << "starting barrel pumping timer";
        t_pumpBarrel.setSingleShot(true);
        t_pumpBarrel.start();
    }



    void AutomaticControl::validateOpenBarrelValve()
    {
        qDebug() << "Validating valve" << m_currentBarrel;
        switch(m_currentBarrel)
        {
            case 1:
                valves()->validateOpenGroup1();
            break;
            case 2:
                valves()->validateOpenGroup2();
            break;
            case 3:
                valves()->validateOpenGroup3();
            break;
            case 4:
               valves()->validateOpenGroup4();
            break;
            case 5:
                valves()->validateOpenGroup5();
            break;
            case 6:
                valves()->validateOpenGroup6();
            break;
            default:
                emit emit_invalidBarrelNumber();
                return;
        }
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
        if((m_currentPump + 6) == 7)
        {
            valves()->validateCloseGroup7();
            return;
        }
        else if((m_currentPump + 6) == 8)
        {
            valves()->validateCloseGroup8();
            return;
        }

        return emit_invalidPumpNumber();
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

        return emit_invalidPumpNumber();
    }


    void AutomaticControl::startManifoldPumpTimer()
    {
        qDebug() << "Manifold pumping timer" << (m_currentPump + 6);
        t_pumpManifold.setSingleShot(true);
        t_pumpManifold.start();
    }


    void AutomaticControl::manifoldLeakDetection()
    {
        qDebug() << "Manifold leak detect" << (m_currentPump + 6);
        m_manifoldLeakDetection.setParams((m_currentPump + 6), 2000, 10, 4);
        m_manifoldLeakDetection.start();
    }


    void AutomaticControl::manifoldPressure()
    {
        // Current barrel reading
        auto currentBarrel = m_pressures[m_currentBarrel];

        // If less than barrel pressure we're good to go
        if(m_pressures[(m_currentPump + 6)] < currentBarrel)
        {
            emit emit_vacuumManifoldSufficent();
            return;
        }

        // Check if the pressure change from previous is none (possiable small leak event)
        if(m_manifoldLastPressure > 0 && ((m_manifoldLastPressure - currentBarrel) > 0))
        {
            emit emit_possiablePumpManifoldLeak();
        }

        // Mark last check
        m_manifoldLastPressure = currentBarrel;

        // Requires more pumping
        emit emit_vacuumManifoldNotSufficent();
    }


    void AutomaticControl::selectBarrel()
    {
        // Increment current by one so we dont repeat it
        if(m_currentBarrel == 6)
        {
            m_currentBarrel = 1;
        }
        else
        {
            m_currentBarrel++;
        }

        // Find next barrel
        for(int a = m_currentBarrel; a <= 6; ++a)
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

                // This barrel needs topping up
                m_currentBarrel = a;

                m_manifoldLastPressure = -1;

                // Break from loop
                break;
            }
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
            t_pumpManifold.setInterval(m_settings->general()->pump(2)["pump_manifold_void"].toInt());

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






