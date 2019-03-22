#include "PumpControl.h"

// Include extenral deps
#include <QObject>


namespace App { namespace Experiment { namespace Machines
{
    PumpControl::PumpControl(QObject *parent, Settings::Container* settings, Hardware::Access& hardware)
        :   Helpers::MachineStates(parent, settings, hardware)

            // Settings container
        ,   m_settings(settings)

            // Timers
        ,   t_warmup(*new QTimer(parent))
        ,   t_pumpVoid(*new QTimer(parent))
        ,   t_pumpManifoldVoid(*new QTimer(parent))

            // Sub state machines
        ,   m_leakDetection(*new LeakDetection(parent, settings, hardware))
    {
        shutDownMachines= false;
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Update params when settings change @TODO

        // Connect states to functions
        connect(state("selectPump", true), &QState::entered, this, &PumpControl::selectPump);

        connect(state("warmupPump", true), &QState::entered, this, &PumpControl::startWarmup);

        connect(validator("vacuumSufficent", true), &QState::entered, this, &PumpControl::isVacuumSufficent);
        connect(validator("vacuumSufficent_2", true), &QState::entered, this, &PumpControl::isVacuumSufficent);

        // Leak state machine
        connect(state("checkForLeaks", true), &QState::entered, this, &PumpControl::leakDetecter);

        // Max timers triggers
        connect(state("startValvePumpTimer", true), &QState::entered, this, &PumpControl::startToValve);
        connect(state("startBarrelPumpTimer", true), &QState::entered, this, &PumpControl::startToBarrel);

        // Are max timers running
        connect(state("maxPumpingValve", true), &QState::entered, this, &PumpControl::isValveTimerRunning);
        connect(state("maxPumpingBarrel", true), &QState::entered, this, &PumpControl::isBarrelTimerRunning);
    }

    PumpControl::~PumpControl()
    {

    }


    /**
     * Set the commands to be used by the machine
     *
     * @brief setParams
     * @param mintues
     * @param turbo
     * @param gasMode
     */
    void PumpControl::setParams(int pump, QString mode)
    {       
        // Time interval for pumps
        params.insert("timeInter", (m_settings->general()->pump(pump)["warm_up"].toInt() * 1000) * 60);
        t_warmup.setInterval(params["timeInter"].toInt());

        // What are enabled?
        params.insert("enable", m_settings->general()->pump(pump)[mode].toBool());

        // Upper set points
        params.insert("upper", m_settings->general()->pump(pump)["upper_set_point"].toInt());

        // Lower set points
        params.insert("lower", m_settings->general()->pump(pump)["lower_set_point"].toInt());

        // Max allowed time for pumping large pump to valve void
        params.insert("pump_void", (m_settings->general()->pump(pump)["pump_void"].toInt() * 1000) * 60);
        t_pumpVoid.setInterval(params["pump_void"].toInt());

        // Max allowed time for pumping large pump to valve to barrel void
        params.insert("pump_manifold_void", (m_settings->general()->pump(pump)["pump_manifold_void"].toInt() * 1000) * 60);
        t_pumpManifoldVoid.setInterval(params["pump_manifold_void"].toInt());

        // Leak detection
        params.insert("period", m_settings->general()->pump(pump)["leak_period"].toInt());
        params.insert("fall", m_settings->general()->pump(pump)["leak_max"].toInt());
        params.insert("sample", 4);

        // Save pump working on
        m_pumpId = pump;

        if(m_pumpId == 1)
        {
            connect(state("pumpOn", true), &QState::entered, this->pumps(), &Functions::PumpFunctions::enablePump1);
            connect(validator("validatePumpOn", true), &QState::entered, this->pumps(), &Functions::PumpFunctions::validateEnablePump1);

            connect(state("pumpOff", true), &QState::entered, this->pumps(), &Functions::PumpFunctions::disablePump1);
            connect(state("valveOff", true), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup7);

            connect(state("openPumpValve", true), &QState::entered, this->valves(), &Functions::ValveFunctions::openGroup7);
            connect(validator("validateOpenPumpValve", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateOpenGroup7);
        }
        else
        {
            qDebug() << "Configure pump 2";

            connect(state("pumpOn", true), &QState::entered, this->pumps(), &Functions::PumpFunctions::enablePump2);
            connect(validator("validatePumpOn", true), &QState::entered, this->pumps(), &Functions::PumpFunctions::validateEnablePump2);

            connect(state("pumpOff", true), &QState::entered, this->pumps(), &Functions::PumpFunctions::disablePump2);
            connect(state("valveOff", true), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup8);

            connect(state("openPumpValve", true), &QState::entered, this->valves(), &Functions::ValveFunctions::openGroup8);
            connect(validator("validateOpenPumpValve", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateOpenGroup8);
        }
    }


    /**
     * Start the state machine
     *
     * @brief PumpControl::start
     */
    void PumpControl::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief PumpControl::stopped
     */
    void PumpControl::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief PumpControl::buildMachine
     */
    void PumpControl::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("selectPump", true));

        // Select pump
        state("selectPump", true)->addTransition(this, &PumpControl::emit_usingPump1, state("pumpOn", true));
        state("selectPump", true)->addTransition(this, &PumpControl::emit_usingPump2, state("pumpOn", true));

            // Turn on pump
            state("pumpOn", true)->addTransition(&m_hardware, &Hardware::Access::emit_pumpEnabled, validator("validatePumpOn", true));
                // validator("validatePumpOn", true)->addTransition(this->pumps(), &Functions::PumpFunctions::emit_Pump2On, state("warmupPump", true));
                validator("validatePumpOn", true)->addTransition(this->pumps(), &Functions::PumpFunctions::emit_validationSuccess, state("warmupPump", true));
                validator("validatePumpOn", true)->addTransition(this->pumps(), &Functions::PumpFunctions::emit_validationWrongId, validator("validatePumpOn", true));
                validator("validatePumpOn", true)->addTransition(this->pumps(), &Functions::PumpFunctions::emit_validationFailed, &sm_stopAsFailed);

                // Warmup pump
                state("warmupPump", true)->addTransition(&t_warmup, &QTimer::timeout, state("startValvePumpTimer", true));

                    // Start timer
                    state("startValvePumpTimer", true)->addTransition(this, &PumpControl::emit_timerStarted, state("guage_pressure", true));

                    // Request read pressure
                    state("guage_pressure", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("vacuumSufficent", true));

                        // Read pressure till hits set point
                        validator("vacuumSufficent", true)->addTransition(this, &PumpControl::emit_vaccumNotSufficient, state("maxPumpingValve", true));
                        validator("vacuumSufficent", true)->addTransition(this, &PumpControl::emit_wrongGuage, state("guage_pressure", true));
                        validator("vacuumSufficent", true)->addTransition(this, &PumpControl::emit_vacuumSufficent, state("openPumpValve", true));

                        // Check max timer
                        state("maxPumpingValve", true)->addTransition(this, &PumpControl::emit_timeElapsed, state("valveOff", true));
                        state("maxPumpingValve", true)->addTransition(this, &PumpControl::emit_timerRunning, state("guage_pressure", true));

                    // Check for leaks here?

                    // Open valve
                    transitionsBuilder()->openValve(state("openPumpValve", true), validator("validateOpenPumpValve", true), state("checkForLeaks", true), state("valveOff", true));

                    // Check for leaks then finish
                    state("checkForLeaks", true)->addTransition(&m_leakDetection, &LeakDetection::emit_machineFinished, state("startBarrelPumpTimer", true));
                    state("checkForLeaks", true)->addTransition(&m_leakDetection, &LeakDetection::emit_leakDetected, state("valveOff", true));

                    // Start max pumping timer
                    state("startBarrelPumpTimer", true)->addTransition(this, &PumpControl::emit_timerStarted, state("guage_pressure_2", true));

                    // Check new pressure
                    state("guage_pressure_2", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("vacuumSufficent_2", true));

                        // Large manifold could increase vacuum above set point; hence read pressure till hits set point
                        validator("vacuumSufficent_2", true)->addTransition(this, &PumpControl::emit_vaccumNotSufficient, state("maxPumpingBarrel", true));
                        validator("vacuumSufficent_2", true)->addTransition(this, &PumpControl::emit_wrongGuage, state("guage_pressure_2", true));
                        validator("vacuumSufficent_2", true)->addTransition(this, &PumpControl::emit_vacuumSufficent, &sm_stop);

                        // Check max timer
                        state("maxPumpingBarrel", true)->addTransition(this, &PumpControl::emit_timeElapsed, state("valveOff", true));
                        state("maxPumpingBarrel", true)->addTransition(this, &PumpControl::emit_timerRunning, state("guage_pressure_2", true));


                    // Before failure turn pump off
                    state("valveOff", true)->addTransition(&m_hardware, &Hardware::Access::emit_valveClosed, state("pumpOff", true));
                    state("pumpOff", true)->addTransition(&m_hardware, &Hardware::Access::emit_pumpDisabled, &sm_stopAsFailed);
    }


    /**
     * start leak detector
     *
     * @brief PumpControl::leakDetecter
     */
    void PumpControl::leakDetecter()
    {
        // Set the params
        m_leakDetection.setParams(m_pumpId + 6, params["period"].toInt(), params["fall"].toInt(), params["sample"].toInt());

        // Build the machine
        m_leakDetection.buildMachine();

        // Start the machine
        m_leakDetection.start();
    }


    /**
     * Stop the leak detector
     *
     * @brief PumpControl::stopVent
     */
    void PumpControl::stopLeakDetecter()
    {
        // Start the machine
        m_leakDetection.cancelStateMachine();
    }


    void PumpControl::selectPump()
    {
        if(m_pumpId == 1)
        {
            emit emit_usingPump1();
            return;
        }

        emit emit_usingPump2();
    }


    void PumpControl::isVacuumSufficent()
    {            
        // Get the validator state instance
        Helpers::CommandValidatorState* command = dynamic_cast<Helpers::CommandValidatorState*>(sender());

        // If cast successfull
        if(command != nullptr && command->package.value("cast_status").toBool())
        {
            // Get the package data from the instance
            QVariantMap package = command->package;

            // Guage id
            int guageId = package["guage_id"].toInt();

            // Check correct valve
            if(guageId != (m_pumpId + 6))
            {
                //qDebug() << "wrong valve:" << guageId << "wanted:"<<(m_pumpId+6);
                emit emit_wrongGuage();
                return;
            }

            // Guage state
            double pressure = package["pressure_mbar"].toDouble();

            // Only update and alert if value has changed
            if(guageId - 6 == m_pumpId && pressure < params["upper"].toDouble())
            {
                // Guage tripped emit
                emit emit_vacuumSufficent();
                //qDebug() << "Yes it is";
                return;
            }
        }

        emit emit_vaccumNotSufficient();
    }


    /**
     * Timer to use to trigger pump warmup
     *
     * @brief PumpControl::startWarmup
     */
    void PumpControl::startWarmup()
    {
        //qDebug() << "Timer started" << t_warmup.interval();
        // Setup timer
        t_warmup.setSingleShot(true);
        t_warmup.start();
    }


    /**
     * Start timer for pump to valve vacuum
     *
     * @brief PumpControl::startToValve
     */
    void PumpControl::startToValve()
    {
        if(t_pumpVoid.isActive())
            return;

        qDebug() << "To valve timer started" << t_pumpVoid.interval();;
        // Setup timer
        t_pumpVoid.setSingleShot(true);
        t_pumpVoid.start();

        emit emit_timerStarted();
    }


    void PumpControl::isValveTimerRunning()
    {
        if(t_pumpVoid.isActive())
        {
            qDebug() << "Valve timer running";
            emit emit_timerRunning();
            return;
        }
        qDebug() << "Valve timer elapsed";
        emit emit_timeElapsed();
    }


    /**
     * Start timer for pump to valve to barrel vacuum
     *
     * @brief PumpControl::startToBarrel
     */
    void PumpControl::startToBarrel()
    {
        if(t_pumpManifoldVoid.isActive())
            return;

        qDebug() << "To barrel timer started"<< t_pumpManifoldVoid.interval();;

        // Setup timer
        t_pumpManifoldVoid.setSingleShot(true);
        t_pumpManifoldVoid.start();

        emit emit_timerStarted();
    }

    void PumpControl::isBarrelTimerRunning()
    {
        if(t_pumpManifoldVoid.isActive())
        {
            qDebug() << "Barrel timer running";
            emit emit_timerRunning();
            return;
        }

         qDebug() << "Barrel timer elapsed";
        emit emit_timeElapsed();
    }
}}}






