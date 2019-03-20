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

        connect(state("openValve1", true), &QState::entered, this->valves(), &Functions::ValveFunctions::openGroup1);
        connect(state("openValve2", true), &QState::entered, this->valves(), &Functions::ValveFunctions::openGroup2);
        connect(state("openValve3", true), &QState::entered, this->valves(), &Functions::ValveFunctions::openGroup3);
        connect(state("openValve4", true), &QState::entered, this->valves(), &Functions::ValveFunctions::openGroup4);
        connect(state("openValve5", true), &QState::entered, this->valves(), &Functions::ValveFunctions::openGroup5);
        connect(state("openValve6", true), &QState::entered, this->valves(), &Functions::ValveFunctions::openGroup6);
        connect(state("openValve7", true), &QState::entered, this->valves(), &Functions::ValveFunctions::openGroup7);
        connect(state("openValve8", true), &QState::entered, this->valves(), &Functions::ValveFunctions::openGroup8);

        connect(validator("openValve1Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateOpenGroup1);
        connect(validator("openValve2Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateOpenGroup2);
        connect(validator("openValve3Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateOpenGroup3);
        connect(validator("openValve4Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateOpenGroup4);
        connect(validator("openValve5Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateOpenGroup5);
        connect(validator("openValve6Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateOpenGroup6);
        connect(validator("openValve7Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateOpenGroup7);
        connect(validator("openValve8Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateOpenGroup8);


        connect(state("closeValve1", true), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup1);
        connect(state("closeValve2", true), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup2);
        connect(state("closeValve3", true), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup3);
        connect(state("closeValve4", true), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup4);
        connect(state("closeValve5", true), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup5);
        connect(state("closeValve6", true), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup6);
        connect(state("closeValve7", true), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup7);
        connect(state("closeValve8", true), &QState::entered, this->valves(), &Functions::ValveFunctions::closeGroup8);

        connect(validator("closeValve1Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup1);
        connect(validator("closeValve2Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup2);
        connect(validator("closeValve3Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup3);
        connect(validator("closeValve4Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup4);
        connect(validator("closeValve5Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup5);
        connect(validator("closeValve6Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup6);
        connect(validator("closeValve7Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup7);
        connect(validator("closeValve8Validate", true), &QState::entered, this->valves(), &Functions::ValveFunctions::validateCloseGroup8);
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

    void ValveControl::valveOne(bool logic)
    {
        if(logic) // Open valve
        {
            // Close all valves
            transitionsBuilder()->closeAllChambers(state("closeChamber1", true),
                                                   validator("closeChamber1Validate", true),
                                                   state("closeChamber2", true),
                                                   validator("closeChamber2Validate", true),
                                                   state("closeChamber3", true),
                                                   validator("closeChamber3Validate", true),
                                                   state("closeChamber4", true),
                                                   validator("closeChamber4Validate", true),
                                                   state("closeChamber5", true),
                                                   validator("closeChamber5Validate", true),
                                                   state("closeChamber6", true),
                                                   validator("closeChamber6Validate", true),
                                                   state("openValve1", true),
                                                   &sm_stopAsFailed);

            // Open valve
            transitionsBuilder()->openValve(state("openValve1", true), validator("openValve1Validate", true), &sm_stop, &sm_stopAsFailed);
        }
        else // Close valve
        {
            transitionsBuilder()->closeValve(state("closeValve1", true), validator("closeValve1Validate", true), &sm_stop, &sm_stopAsFailed);
        }
    }

    void ValveControl::valveTwo(bool logic)
    {
        if(logic) // Open valve
        {
            // Close all valves
            transitionsBuilder()->closeAllChambers(state("closeChamber1", true),
                                                   validator("closeChamber1Validate", true),
                                                   state("closeChamber2", true),
                                                   validator("closeChamber2Validate", true),
                                                   state("closeChamber3", true),
                                                   validator("closeChamber3Validate", true),
                                                   state("closeChamber4", true),
                                                   validator("closeChamber4Validate", true),
                                                   state("closeChamber5", true),
                                                   validator("closeChamber5Validate", true),
                                                   state("closeChamber6", true),
                                                   validator("closeChamber6Validate", true),
                                                   state("openValve2", true),
                                                   &sm_stopAsFailed);

            // Open valve
            transitionsBuilder()->openValve(state("openValve2", true), validator("openValve2Validate", true), &sm_stop, &sm_stopAsFailed);
        }
        else // Close valve
        {
            transitionsBuilder()->closeValve(state("closeValve2", true), validator("closeValve2Validate", true), &sm_stop, &sm_stopAsFailed);
        }
    }

    void ValveControl::valveThree(bool logic)
    {
        if(logic) // Open valve
        {
            // Close all valves
            transitionsBuilder()->closeAllChambers(state("closeChamber1", true),
                                                   validator("closeChamber1Validate", true),
                                                   state("closeChamber2", true),
                                                   validator("closeChamber2Validate", true),
                                                   state("closeChamber3", true),
                                                   validator("closeChamber3Validate", true),
                                                   state("closeChamber4", true),
                                                   validator("closeChamber4Validate", true),
                                                   state("closeChamber5", true),
                                                   validator("closeChamber5Validate", true),
                                                   state("closeChamber6", true),
                                                   validator("closeChamber6Validate", true),
                                                   state("openValve3", true),
                                                   &sm_stopAsFailed);

            // Open valve
            transitionsBuilder()->openValve(state("openValve3", true), validator("openValve3Validate", true), &sm_stop, &sm_stopAsFailed);
        }
        else // Close valve
        {
            transitionsBuilder()->closeValve(state("closeValve3", true), validator("closeValve3Validate", true), &sm_stop, &sm_stopAsFailed);
        }
    }

    void ValveControl::valveFour(bool logic)
    {
        if(logic) // Open valve
        {
            // Close all valves
            transitionsBuilder()->closeAllChambers(state("closeChamber1", true),
                                                   validator("closeChamber1Validate", true),
                                                   state("closeChamber2", true),
                                                   validator("closeChamber2Validate", true),
                                                   state("closeChamber3", true),
                                                   validator("closeChamber3Validate", true),
                                                   state("closeChamber4", true),
                                                   validator("closeChamber4Validate", true),
                                                   state("closeChamber5", true),
                                                   validator("closeChamber5Validate", true),
                                                   state("closeChamber6", true),
                                                   validator("closeChamber6Validate", true),
                                                   state("openValve4", true),
                                                   &sm_stopAsFailed);

            // Open valve
            transitionsBuilder()->openValve(state("openValve4", true), validator("openValve4Validate", true), &sm_stop, &sm_stopAsFailed);
        }
        else // Close valve
        {
            transitionsBuilder()->closeValve(state("closeValve4", true), validator("closeValve4Validate", true), &sm_stop, &sm_stopAsFailed);
        }
    }

    void ValveControl::valveFive(bool logic)
    {
        if(logic) // Open valve
        {
            // Close all valves
            transitionsBuilder()->closeAllChambers(state("closeChamber1", true),
                                                   validator("closeChamber1Validate", true),
                                                   state("closeChamber2", true),
                                                   validator("closeChamber2Validate", true),
                                                   state("closeChamber3", true),
                                                   validator("closeChamber3Validate", true),
                                                   state("closeChamber4", true),
                                                   validator("closeChamber4Validate", true),
                                                   state("closeChamber5", true),
                                                   validator("closeChamber5Validate", true),
                                                   state("closeChamber6", true),
                                                   validator("closeChamber6Validate", true),
                                                   state("openValve5", true),
                                                   &sm_stopAsFailed);

            // Open valve
            transitionsBuilder()->openValve(state("openValve5", true), validator("openValve5Validate", true), &sm_stop, &sm_stopAsFailed);
        }
        else // Close valve
        {
            transitionsBuilder()->closeValve(state("closeValve5", true), validator("closeValve5Validate", true), &sm_stop, &sm_stopAsFailed);
        }
    }

    void ValveControl::valveSix(bool logic)
    {
        if(logic) // Open valve
        {
            // Close all valves
            transitionsBuilder()->closeAllChambers(state("closeChamber1", true),
                                                   validator("closeChamber1Validate", true),
                                                   state("closeChamber2", true),
                                                   validator("closeChamber2Validate", true),
                                                   state("closeChamber3", true),
                                                   validator("closeChamber3Validate", true),
                                                   state("closeChamber4", true),
                                                   validator("closeChamber4Validate", true),
                                                   state("closeChamber5", true),
                                                   validator("closeChamber5Validate", true),
                                                   state("closeChamber6", true),
                                                   validator("closeChamber6Validate", true),
                                                   state("openValve6", true),
                                                   &sm_stopAsFailed);

            // Open valve
            transitionsBuilder()->openValve(state("openValve6", true), validator("openValve6Validate", true), &sm_stop, &sm_stopAsFailed);
        }
        else // Close valve
        {
            transitionsBuilder()->closeValve(state("closeValve6", true), validator("closeValve6Validate", true), &sm_stop, &sm_stopAsFailed);
        }
    }

    void ValveControl::valveSeven(bool logic)
    {
        // Open valve
        if(logic)
        {
            transitionsBuilder()->closeValve(state("closeValve8", true), validator("closeValve8Validate", true), state("openValve7", true), &sm_stopAsFailed);

            transitionsBuilder()->openValve(state("openValve7", true), validator("openValve7Validate", true), &sm_stop, &sm_stopAsFailed);

        }
        else // Close valvestate
        {
            transitionsBuilder()->closeValve(state("closeValve7", true), validator("closeValve7Validate", true), &sm_stop, &sm_stopAsFailed);
        }
    }

    void ValveControl::valveEight(bool logic)
    {
        // Open valve
        if(logic)
        {
            transitionsBuilder()->closeValve(state("closeValve7", true), validator("closeValve7Validate", true), state("openValve8", true), &sm_stopAsFailed);

            transitionsBuilder()->openValve(state("openValve8", true), validator("openValve8Validate", true), &sm_stop, &sm_stopAsFailed);

        }
        else // Close valvestate
        {
            transitionsBuilder()->closeValve(state("closeValve8", true), validator("closeValve8Validate", true), &sm_stop, &sm_stopAsFailed);
        }
    }

}}}






