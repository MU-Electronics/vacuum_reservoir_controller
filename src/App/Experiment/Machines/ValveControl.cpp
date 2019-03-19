#include "ValveControl.h"

// Include extenral deps
#include <QObject>


namespace App { namespace Experiment { namespace Machines
{
    ValveControl::ValveControl(QObject *parent, Settings::Container* settings, Hardware::Access& hardware)
        :   Helpers::MachineStates(parent, settings, hardware)

            // Settings container
        ,   m_settings(settings)

    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Update params when settings change @TODO
    }

    ValveControl::~ValveControl()
    {

    }


    /**
     * Set the commands to be used by the machine
     *
     */
    void ValveControl::setParams(int valveId)
    {       
        // Save valve working on
        m_valve = valveId;
    }


    /**
     * Start the state machine
     *
     * @brief ValveControl::start
     */
    void ValveControl::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief ValveControl::stopped
     */
    void ValveControl::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief ValveControl::buildMachine
     */
    void ValveControl::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("selectPump", true));

   }

}}}






