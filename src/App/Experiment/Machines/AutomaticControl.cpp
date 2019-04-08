#include "AutomaticControl.h"

#include "Hal/time_api.h"

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
        ,   t_continuousLeakDetection(*new QTimer(parent))

            // state machines
        ,   m_pumpController(pumpController)
        ,   m_manifoldLeakDetection(*new LeakDetection(parent, settings, hardware))
        ,   m_barrelLeakDetection(*new LeakDetection(parent, settings, hardware))
        ,   m_pumpingLeakDetection(*new LeakDetection(parent, settings, hardware))
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Set default barrel interval
        t_pumpBarrel.setInterval(10000);

        // How often should the global leak detect occure
        t_continuousLeakDetection.setInterval(m_continuousLeakPeriod);
        t_continuousLeakDetection.setSingleShot(false);

        // Enable the shutdown state machine
        shutDownMachines = true;

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
        connect(state("failureOpenPumpValve", true), &QState::entered, this, &AutomaticControl::openPumpValve);

        connect(state("openPumpValve", true), &QState::entered, this, &AutomaticControl::openPumpValve);
        connect(state("pumpingBarrelLeakDetect", true), &QState::entered, this, &AutomaticControl::pumpingBarrelLeakDetect);
        connect(state("closeBarrelValve", true), &QState::entered, this, &AutomaticControl::closeBarrelValve);

        connect(state("turnValveOffFailure", true), &QState::entered, this, &AutomaticControl::closePumpValve);
        connect(validator("turnValveOffFailure", true), &QState::entered, this, &AutomaticControl::validateClosePumpValve);

        connect(state("turnPumpOffFailure", true), &QState::entered, this, &AutomaticControl::shutdownPump);
        connect(validator("turnPumpOffFailure", true), &QState::entered, this, &AutomaticControl::validateShutdownPump);



        connect(&t_continuousLeakDetection, &QTimer::timeout, this, &AutomaticControl::continuousLeakDetection);



        // Connect for shut down state machine states to functions
        connect(state("pumpController", false), &QState::entered, this, &AutomaticControl::shutdownPumpController);
        connect(state("barrelLeak", false), &QState::entered, this, &AutomaticControl::shutdownBarrelLeakDetector);
        connect(state("manifoldLeak", false), &QState::entered, this, &AutomaticControl::shutdownManiFoldLeakDetector);
        connect(state("pumpingLeak", false), &QState::entered, this, &AutomaticControl::shutdownPumpingLeakDetector);

        connect(state("closeChamber1", false), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup1);
        connect(state("closeChamber2", false), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup2);
        connect(state("closeChamber3", false), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup3);
        connect(state("closeChamber4", false), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup4);
        connect(state("closeChamber5", false), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup5);
        connect(state("closeChamber6", false), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup6);

        connect(validator("closeChamber1Validate", false), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup1);
        connect(validator("closeChamber2Validate", false), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup2);
        connect(validator("closeChamber3Validate", false), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup3);
        connect(validator("closeChamber4Validate", false), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup4);
        connect(validator("closeChamber5Validate", false), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup5);
        connect(validator("closeChamber6Validate", false), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup6);

        connect(state("valveOff1", false), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup7);
        connect(state("valveOff2", false), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup8);

        connect(state("pumpOff1", false), &QState::entered, this->pumps(), &Functions::PumpFunctions::disablePump1);
        connect(state("pumpOff2", false), &QState::entered, this->pumps(), &Functions::PumpFunctions::disablePump2);
    }

    AutomaticControl::~AutomaticControl()
    {

    }


    /**
     * Set the commands to be used by the machine
     *
     */
    void AutomaticControl::setParams(bool ignoreCurrentTrips)
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

        // Should we account for current guage trips?
        if(ignoreCurrentTrips)
            m_tripped.clear();

        // Reset old leak& heavy load params
        m_leaked.clear();
        m_heavyLoad.clear();

        // Set starting pump position
        m_currentPump = (m_settings->general()->defaultPump() == 1) ? 0 : 1;
    }


    /**
     * Start the state machine
     *
     * @brief AutomaticControl::start
     */
    void AutomaticControl::beforeStart()
    {
        t_continuousLeakDetection.start();
    }


    /**
     * Start the state machine
     *
     * @brief AutomaticControl::stopped
     */
    void AutomaticControl::stopped()
    {
        t_pumpManifold.stop();
        t_pumpBarrel.stop();
        t_continuousLeakDetection.stop();
    }

    void AutomaticControl::buildShutDownMachine()
    {
        // Stop pump controller m_pumpController
        shutDownMachine.setInitialState(state("pumpController", false));

        // Stop leak detector m_manifoldLeakDetection
        state("pumpController", false)->addTransition(&m_pumpController, &PumpControl::emit_machineFinished, state("barrelLeak", false));
        state("pumpController", false)->addTransition(&m_pumpController, &PumpControl::emit_machineFailed, state("barrelLeak", false));
        state("pumpController", false)->addTransition(this, &AutomaticControl::emit_machineAlreadyStopped, state("barrelLeak", false));

        // Stop leak detector m_barrelLeakDetection
        state("barrelLeak", false)->addTransition(&m_barrelLeakDetection, &LeakDetection::emit_machineFinished, state("manifoldLeak", false));
        state("barrelLeak", false)->addTransition(&m_barrelLeakDetection, &LeakDetection::emit_machineFailed, state("manifoldLeak", false));
        state("barrelLeak", false)->addTransition(this, &AutomaticControl::emit_machineAlreadyStopped, state("manifoldLeak", false));

        // Stop manifold leak detector
        state("manifoldLeak", false)->addTransition(&m_manifoldLeakDetection, &LeakDetection::emit_machineFinished, state("pumpingLeak", false));
        state("manifoldLeak", false)->addTransition(&m_manifoldLeakDetection, &LeakDetection::emit_machineFailed, state("pumpingLeak", false));
        state("manifoldLeak", false)->addTransition(this, &AutomaticControl::emit_machineAlreadyStopped, state("pumpingLeak", false));

        // Stop pumping leak detector
        state("pumpingLeak", false)->addTransition(&m_pumpingLeakDetection, &LeakDetection::emit_machineFinished, state("closeChamber1", false));
        state("pumpingLeak", false)->addTransition(&m_pumpingLeakDetection, &LeakDetection::emit_machineFailed, state("closeChamber1", false));
        state("pumpingLeak", false)->addTransition(this, &AutomaticControl::emit_machineAlreadyStopped, state("closeChamber1", false));

        // Close all barrel valves
        transitionsBuilder()->closeAllChambers(state("closeChamber1", false),
                                               validator("closeChamber1Validate", false),
                                               state("closeChamber2", false),
                                               validator("closeChamber2Validate", false),
                                               state("closeChamber3", false),
                                               validator("closeChamber3Validate", false),
                                               state("closeChamber4", false),
                                               validator("closeChamber4Validate", false),
                                               state("closeChamber5", false),
                                               validator("closeChamber5Validate", false),
                                               state("closeChamber6", false),
                                               validator("closeChamber6Validate", false),
                                               state("valveOff1", false),
                                               state("valveOff1", false));

        state("valveOff1", false)->addTransition(&m_hardware, &Hardware::Access::emit_valveClosed, state("valveOff2", false));
        state("valveOff2", false)->addTransition(&m_hardware, &Hardware::Access::emit_valveClosed, state("pumpOff1", false));

        state("pumpOff1", false)->addTransition(&m_hardware, &Hardware::Access::emit_pumpDisabled, state("pumpOff2", false));
        state("pumpOff2", false)->addTransition(&m_hardware, &Hardware::Access::emit_pumpDisabled, &ssm_stop);

    }


    void AutomaticControl::shutdownPumpController()
    {
        qDebug() << "Stopping pump controller";
        if(m_pumpController.machine.isRunning())
        {
            m_pumpController.cancelStateMachine();
            return;
        }

        emit emit_machineAlreadyStopped();
    }

    void AutomaticControl::shutdownBarrelLeakDetector()
    {
        qDebug() << "Stopping shutdown barrel leak dector";
        if(m_barrelLeakDetection.machine.isRunning())
        {
            m_barrelLeakDetection.cancelStateMachine();
            return;
        }

        emit emit_machineAlreadyStopped();
    }

    void AutomaticControl::shutdownManiFoldLeakDetector()
    {
        qDebug() << "Stopping shutdown manifold leak dector";
        if(m_manifoldLeakDetection.machine.isRunning())
        {
            m_manifoldLeakDetection.cancelStateMachine();
            return;
        }

        emit emit_machineAlreadyStopped();
    }

    void AutomaticControl::shutdownPumpingLeakDetector()
    {
        qDebug() << "Stopping shutdown pumping leak dector";
        if(m_pumpingLeakDetection.machine.isRunning())
        {
            m_pumpingLeakDetection.cancelStateMachine();
            return;
        }

        emit emit_machineAlreadyStopped();
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
        state("markPumpFailure", true)->addTransition(this, &AutomaticControl::emit_pumpMarkedAsFailed, state("turnValveOffFailure", true));
            state("turnValveOffFailure", true)->addTransition(&m_hardware, &Hardware::Access::emit_valveClosed, state("turnPumpOffFailure", true));
            state("turnPumpOffFailure", true)->addTransition(&m_hardware, &Hardware::Access::emit_pumpDisabled, state("selectPump", true));

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
            state("failureOpenPumpValve", true)->addTransition(&m_hardware, &Hardware::Access::emit_valveOpened, state("noBarrelsWait", true));


        // Open pump valve and start pumping @todo needs validation
        state("openPumpValve", true)->addTransition(&m_hardware, &Hardware::Access::emit_valveOpened, state("pumpingBarrelLeakDetect", true));

        // Do some pumping with leak detection
        state("pumpingBarrelLeakDetect", true)->addTransition(&m_pumpingLeakDetection, &LeakDetection::emit_machineFailed, state("closeBarrelValve", true));
        state("pumpingBarrelLeakDetect", true)->addTransition(&m_pumpingLeakDetection, &LeakDetection::emit_machineFinished, state("closeBarrelValve", true));

        // Do other barrels need pumping if not we could continue the current barrel? @ TODO


        // Close barrel valve return to select barrel @todo needs validation
        state("closeBarrelValve", true)->addTransition(&m_hardware, &Hardware::Access::emit_valveClosed, state("noBarrelsWait", true));
    }

    // @todo remove timestamp not required
    void AutomaticControl::continuousLeakDetection()
    {
        // Check for leak and heavy load in each chamber
        for(auto chamber: m_enabled)
        {
            // Check if leak detect is disabled in settings
            if(chamber <= 6 && m_settings->general()->chamber(chamber)["leak_detection"].toBool())
                continue;
            if(chamber >= 7 && m_settings->general()->pump(chamber - 6)["leak_detection"].toBool())
                continue;

            // Check if the chamber is not disabled via previous error already
            if(!m_error.contains(chamber) && !m_tripped.contains(chamber) && !m_leaked.contains(chamber))
            {
                // Create a new reading record
                QMap<QString, QVariant> value;
                value["time"] = millis();
                value["value"] =  m_pressures[chamber];

                // First reading with timestamp
                if(!m_majorLeakMonitoring.contains(chamber) && m_pressures.contains(chamber))
                {
                    m_majorLeakMonitoring[chamber] = value;
                    return;
                }

                // Get chamber info
                auto settings = m_settings->general()->chamber(chamber);

                // Calculations
//                double pressureTolerance = value["value"].toDouble() * 0.15;
//                double pressureDrop = (value["value"].toDouble() - m_majorLeakMonitoring[chamber]["value"].toDouble()) - pressureTolerance;
//                double maxFall = (settings["leak_max"].toDouble() / settings["leak_period"].toInt()) * m_continuousLeakPeriod;
//                double heavyLoadFall = (settings["heavy_load"].toDouble() / settings["leak_period"].toInt()) * m_continuousLeakPeriod;

                double pressureDrop = (value["value"].toDouble() - m_majorLeakMonitoring[chamber]["value"].toDouble());
                double maxFall = settings["leak_max"].toDouble();
                double heavyLoadFall = settings["heavy_load"].toDouble();

                //qDebug() << value["value"].toDouble() << m_majorLeakMonitoring[chamber]["value"].toDouble() << chamber << pressureDrop << maxFall << heavyLoadFall << (value["value"].toDouble() * 0.15);
                // Compare for a leak threashold
                if(pressureDrop > maxFall)
                {
                    qDebug() << "Leak detected in chamber" << chamber << pressureDrop << maxFall;
                }
                else if(pressureDrop > heavyLoadFall) // Compare for a heavy load threashold
                {
                    qDebug() << "Heavy load detected in chamber" << chamber << pressureDrop << heavyLoadFall;
                }

                // Update old value
                m_majorLeakMonitoring[chamber] = value;
            }
        }

    }


    void AutomaticControl::shutdownPump()
    {
        if(m_currentPump == 1)
        {
           this->pumps()->disablePump1();
        }
        else if(m_currentPump == 2)
        {
            this->pumps()->disablePump2();
        }
    }


    void AutomaticControl::validateShutdownPump()
    {

    }


    void AutomaticControl::pumpingBarrelLeakDetect()
    {
        // Pumping
        auto chamber = m_settings->general()->chamber(m_currentBarrel);
        m_pumpingLeakDetection.setParams(m_currentBarrel, chamber["pumping_time"].toInt(), chamber["leak_max"].toDouble(), (chamber["barrel_delay"].toInt() / 1000), chamber["barrel_delay"].toInt());
        m_pumpingLeakDetection.start();
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

    void AutomaticControl::openPumpValve()
    {
        qDebug() << "Opening pump valve" << (m_currentPump + 6);
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

        m_barrelLeakDetection.setParams(m_currentBarrel, barrel["leak_period"].toInt(), barrel["leak_max"].toInt(), (barrel["leak_delay"].toInt() / 1000), barrel["leak_delay"].toInt());
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
        auto pump = m_settings->general()->pump(m_currentPump);
        m_manifoldLeakDetection.setParams((m_currentPump + 6), pump["leak_period"].toInt(), pump["leak_max"].toDouble(), (pump["manifold_delay"].toInt() / 1000), pump["manifold_delay"].toInt());
        m_manifoldLeakDetection.start();
    }


    void AutomaticControl::manifoldPressure()
    {
        // Current barrel reading
        auto currentBarrel = m_pressures[m_currentBarrel];
        auto currentPump  = m_pressures[m_currentPump + 6];

        qDebug() <<"Mani pressure: "<< currentPump << "Barrel Pressure:" << currentBarrel;


        if(!m_manifoldTimerStarted)
        {
            qDebug() << "Starting mani timer" << t_pumpManifold.interval();
            t_pumpManifold.setSingleShot(true);
            t_pumpManifold.start();
            m_manifoldTimerStarted = true;
        }

        // If less than barrel pressure we're good to go
        if(currentBarrel > currentPump)
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
        m_manifoldLastPressure = currentPump;

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

        //qDebug() << "max:"<<maxBarrelId<<"Min"<<minBarrelId<<"starting at"<<startingPoint;

        // Find next barrel
        int foundBarrel = false;
        for(int a = startingPoint; a <= maxBarrelId; ++a)
        {
            if(m_enabled.contains(a))
            {
                // Check barrel pressure above lower set point (pressure in mbar, settings in mbar)
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

                if(m_error.contains(a))
                    continue;

                // We have found a barrel that we can work on next
                foundBarrel = true;

                // This barrel needs topping up
                m_currentBarrel = a;

                // Set pumping time for barrel pumping_time
                t_pumpBarrel.setInterval(m_settings->general()->chamber(a)["pumping_time"].toInt());

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
        // Take data we need form package
        int group = package.value("guage_id").toInt();
        double value = package.value("pressure_mbar").toDouble();
        int error = package.value("view_status").toInt();

        // Save current pressure value
        m_pressures.insert(group, value);

        // Was there an error in reading of the guage
        int find = m_error.indexOf(group);
        if(error == 3 && find != -1)
        {
            m_error.append(group);
        }
        else
        {
            m_error.removeAt(find);
        }
    }


    void majorLeakDetection()
    {

    }

}}}






