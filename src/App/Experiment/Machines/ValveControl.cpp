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
    void ValveControl::setParams(int valveId, bool state)
    {       
        // Save valve working on
        m_valve = valveId;
        m_state = state;
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
        // Build and run the correct state machine
        if(m_valve == 1)
            valveOne(m_state);
        if(m_valve == 2)
            valveTwo(m_state);
        if(m_valve == 3)
            valveThree(m_state);
        if(m_valve == 4)
            valveFour(m_state);
        if(m_valve == 5)
            valveFive(m_state);
        if(m_valve == 6)
            valveSix(m_state);
        if(m_valve == 7)
            valveSeven(m_state);
        if(m_valve == 8)
            valveEight(m_state);
    }

    void ValveControl::closeAllChambers(QState* returnTo)
    {


    }

    void ValveControl::valveOne(bool state)
    {
        // Open valve
        if(state)
        {
            // Close all valves

            // Open valve
        }
        else // Close valve
        {

        }
    }

    void ValveControl::valveTwo(bool state)
    {
        // Open valve
        if(state)
        {

        }
        else // Close valve
        {

        }
    }

    void ValveControl::valveThree(bool state)
    {
        // Open valve
        if(state)
        {

        }
        else // Close valve
        {

        }
    }

    void ValveControl::valveFour(bool state)
    {
        // Open valve
        if(state)
        {

        }
        else // Close valve
        {

        }
    }

    void ValveControl::valveFive(bool state)
    {
        // Open valve
        if(state)
        {

        }
        else // Close valve
        {

        }
    }

    void ValveControl::valveSix(bool state)
    {
        // Open valve
        if(state)
        {

        }
        else // Close valve
        {

        }
    }

    void ValveControl::valveSeven(bool state)
    {
        // Open valve
        if(state)
        {

        }
        else // Close valve
        {

        }
    }

    void ValveControl::valveEight(bool state)
    {
        // Open valve
        if(state)
        {

        }
        else // Close valve
        {

        }
    }

}}}






