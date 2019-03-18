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

        // Pump warmup
            // Read pressure till hits set point
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






