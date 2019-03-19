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
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Update params when settings change @TODO

        // Connect states to functions
        connect(state("selectPump", true), &QState::entered, this, &PumpControl::selectPump);

        connect(state("turnPumpOneOn", true), &QState::entered, this->pumps(), &Functions::PumpFunctions::enablePump1);
        connect(state("turnPumpTwoOn", true), &QState::entered, this->pumps(), &Functions::PumpFunctions::enablePump2);

        connect(state("validateTurnPumpOneOn", true), &QState::entered, this->pumps(), &Functions::PumpFunctions::validateEnablePump1);
        connect(state("validateTurnPumpTwoOn", true), &QState::entered, this->pumps(), &Functions::PumpFunctions::validateEnablePump2);

        connect(state("warmupPumpOne", true), &QState::entered, this, &PumpControl::startWarmup);
        connect(state("warmupPumpOne", true), &QState::entered, this, &PumpControl::startWarmup);

        connect(validator("vacuumSufficent", true), &QState::entered, this, &PumpControl::isVacuumSufficent);

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
        params.insert("timeInter", m_settings->general()->pump(pump)["warm_up"].toInt());
        t_warmup.setInterval(params["timeInter"].toInt());

        // What are enabled?
        params.insert("enable", m_settings->general()->pump(pump)[mode].toBool());

        // Upper set points
        params.insert("upper", m_settings->general()->pump(pump)["upper_set_point"].toInt());

        // Lower set points
        params.insert("lower", m_settings->general()->pump(pump)["lower_set_point"].toInt());

        // Save pump working on
        m_pumpId = pump;
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
        state("selectPump", true)->addTransition(this, &PumpControl::emit_usingPump1, state("turnPumpOneOn", true));
        state("selectPump", true)->addTransition(this, &PumpControl::emit_usingPump2, state("turnPumpTwoOn", true));

            // Turn on pump
            state("turnPumpOneOn", true)->addTransition(&m_hardware, &Hardware::Access::emit_pumpEnabled, validator("validateTurnPumpOneOn", true));
            state("turnPumpTwoOn", true)->addTransition(&m_hardware, &Hardware::Access::emit_pumpEnabled, validator("validateTurnPumpTwoOn", true));
                validator("validateTurnPumpOneOn", true)->addTransition(this->pumps(), &Functions::PumpFunctions::emit_Pump1On, state("warmupPumpOne", true));
                validator("validateTurnPumpOneOn", true)->addTransition(this->pumps(), &Functions::PumpFunctions::emit_validationWrongId, validator("validateTurnPumpOneOn", true));
                validator("validateTurnPumpOneOn", true)->addTransition(this->pumps(), &Functions::PumpFunctions::emit_validationFailed, &sm_stopAsFailed);

                validator("validateTurnPumpTwoOn", true)->addTransition(this->pumps(), &Functions::PumpFunctions::emit_Pump2On, state("warmupPumpTwo", true));
                validator("validateTurnPumpTwoOn", true)->addTransition(this->pumps(), &Functions::PumpFunctions::emit_validationFailed, &sm_stopAsFailed);
                validator("validateTurnPumpTwoOn", true)->addTransition(this->pumps(), &Functions::PumpFunctions::emit_validationWrongId, validator("validateTurnPumpTwoOn", true));

                // Warmup pump
                state("warmupPumpOne", true)->addTransition(&t_warmup, &QTimer::timeout, state("vacuumSufficent", true));
                state("warmupPumpTwo", true)->addTransition(&t_warmup, &QTimer::timeout, state("vacuumSufficent", true));

                    // Read pressure till hits set point
                    validator("vacuumSufficent", true)->addTransition(this, &PumpControl::emit_vaccumNotSufficient, validator("vacuumSufficent", true));
                    validator("vacuumSufficent", true)->addTransition(this, &PumpControl::emit_vacuumSufficent, state("openValve", true));

                        // Open valve

                            // Wait for pressure to equalise

                                // Check for leaks then finish
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
            int guageId = package["requested_gauge_group"].toInt();

            // Guage state
            double pressure = package["pressure"].toDouble();

            // Only update and alert if value has changed
            if(guageId - 6 == m_pumpId && pressure < params["upper"].toDouble())
            {
                // Guage tripped emit
                emit emit_vacuumSufficent();
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
        // Setup timer
        t_warmup.setSingleShot(true);
        t_warmup.start();
    }
}}}






