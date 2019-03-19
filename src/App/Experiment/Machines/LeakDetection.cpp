#include "LeakDetection.h"

// Include extenral deps
#include <QObject>


namespace App { namespace Experiment { namespace Machines
{
    LeakDetection::LeakDetection(QObject *parent, Settings::Container* settings, Hardware::Access& hardware)
        :   Helpers::MachineStates(parent, settings, hardware)

            // Settings container
        ,   m_settings(settings)

            // Timers
        ,   t_leakPeriod(*new QTimer(parent))
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Update params when settings change @TODO


    }

    LeakDetection::~LeakDetection()
    {

    }


    /**
     * Set the commands to be used by the machine
     *
     */
    void LeakDetection::setParams(int period, int group)
    {       
        // Time interval for pumps
        params.insert("leak_period", period);
        t_leakPeriod.setInterval(period);

        // Save pump working on
        m_period = period;
        m_group = group;
    }


    /**
     * Start the state machine
     *
     * @brief LeakDetection::start
     */
    void LeakDetection::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief LeakDetection::stopped
     */
    void LeakDetection::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief LeakDetection::buildMachine
     */
    void LeakDetection::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("selectPump", true));

   }



    /**
     * Timer to use to trigger pump warmup
     *
     * @brief LeakDetection::startWarmup
     */
    void LeakDetection::startLeakPeriod()
    {
        // Setup timer
        t_leakPeriod.setSingleShot(true);
        t_leakPeriod.start();
    }
}}}






