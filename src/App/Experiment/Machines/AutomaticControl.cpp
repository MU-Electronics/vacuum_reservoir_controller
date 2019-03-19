#include "AutomaticControl.h"

// Include extenral deps
#include <QObject>


namespace App { namespace Experiment { namespace Machines
{
    AutomaticControl::AutomaticControl(QObject *parent, Settings::Container* settings, Hardware::Access& hardware)
        :   Helpers::MachineStates(parent, settings, hardware)

            // Settings container
        ,   m_settings(settings)
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Update params when settings change @TODO


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

   }

}}}






