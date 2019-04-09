#include "MachineStates.h"

#include <cmath>

// Include extenral deps
#include <QObject>
#include <QDebug>
#include <QVariantMap>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include valaitor state
#include "CommandValidatorState.h"


namespace App { namespace Experiment { namespace Machines { namespace Helpers
{

    MachineStates::MachineStates(QObject *parent, Settings::Container* settings, Hardware::Access &hardware)
        :   QObject(parent)

            // Access the global referances
        ,   m_settings(settings)
        ,   m_hardware(hardware)

            // Access the the hardware command builder
        ,   m_commandConstructor(*new Hardware::CommandConstructor)

            // Init the two state machines (start and stop)
        ,   machine(parent)
        ,   shutDownMachine(parent)

            // Function classes for the states to connect to
        ,   m_valves(new Functions::ValveFunctions(parent, settings, hardware, machine, params, m_commandConstructor))
        ,   m_guages(new Functions::GuageFunctions(parent, settings, hardware, machine, params, m_commandConstructor))
        ,   m_emergancyStop(new Functions::EmergancyStopFunctions(parent, settings, hardware, machine, params, m_commandConstructor))
        ,   m_pumps(new Functions::PumpFunctions(parent, settings, hardware, machine, params, m_commandConstructor))
        ,   m_remote(new Functions::RemoteFunctions(parent, settings, hardware, machine, params, m_commandConstructor))
        ,   m_temperatureSensor(new Functions::TemperatureSensorFunctions(parent, settings, hardware, machine, params, m_commandConstructor))

            // Builders to build transition blocks for common state machine logic
        ,   m_transitionsBuilder(new TransitionsBuilder(parent, settings, hardware, m_emergancyStop, m_valves, m_pumps, m_guages, m_remote, m_temperatureSensor))

        ,   // Main state for machine
            sm_master(&machine),
            sm_stop_2(&machine)

            // States for stopping with success and failure for start state machine
        ,   sm_stop(&sm_master)
        ,   sm_stopAsFailed(&sm_master)

            // States for stopping with success and failure for stop state machine
        ,   ssm_stop(&shutDownMachine)
        ,   ssm_stopAsFailed(&shutDownMachine)
    {
        // Connect object signals to hardware slots and visa versa
        // connect(this, &MachineStates::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }

    MachineStates::~MachineStates()
    {
        // Delete the external state pointers

    }



    /**
     * Connect states to their matching methods
     *      NOTE: Methods are virtual so can be overloaded in machine classes
     *
     * @brief MachineStates::connectStatesToMethods
     */
    void MachineStates::connectStatesToMethods()
    {
        // Connect cancel state machine signal
        connect(&machine, &QStateMachine::finished, this, &MachineStates::stopMachineWithoutError, Qt::UniqueConnection);

        // Tell the machine to stop becuase of success or error
        connect(&sm_stop, &QState::entered, this, &MachineStates::stopMachineWithoutError, Qt::UniqueConnection);
        connect(&sm_stopAsFailed, &QState::entered, this, &MachineStates::stopMachineWithError, Qt::UniqueConnection);

        // When machine has stopped running the stopped method in each machine
        connect(&machine, &QStateMachine::stopped, this, &MachineStates::emitStopped, Qt::UniqueConnection);
        connect(this, &MachineStates::emit_machineAlreadyStopped, this, &MachineStates::emitStopped, Qt::UniqueConnection);

        // Shut down sub state machines
        connect(&ssm_stop, &QState::entered, this, &MachineStates::stopShutDownSubMachineWithoutError, Qt::UniqueConnection);
        connect(&ssm_stopAsFailed, &QState::entered, this, &MachineStates::stopShutDownSubMachineWithError, Qt::UniqueConnection);
        connect(&shutDownMachine, &QStateMachine::stopped, this, &MachineStates::afterSubMachinesStopped, Qt::UniqueConnection);
    }


    // @TODO ADD GETTERS FOR FUNCION CLASSES
    


    /**
     * Returns the transistion builder instance
     *
     * @brief MachineStates::transitionsBuilder
     * @return
     */
    TransitionsBuilder* MachineStates::transitionsBuilder()
    {
        return m_transitionsBuilder;
    }


    /**
     * Proccess in starting the machine
     *
     * @brief MachineStates::start
     */
    void MachineStates::start()
    {
        // Run local before start function
        beforeStart();

        // Remove old transitions
        removeAllTransitions(machine);
        removeAllTransitions(shutDownMachine);

        machine.setInitialState(&sm_master);
        sm_master.addTransition(this, &MachineStates::emit_cancelMachine, &sm_stop_2);

        // Build new transitions
        buildMachine();

        // Start machine
        machine.start();
    }


    /**
     * Method that triggers cancellation of state machine
     *
     * @brief MachineStates::cancelStateMachine
     */
    void MachineStates::cancelStateMachine()
    {
        emit emit_cancelMachine();
    }


    /**
     * Runs after sub machines have been stopped if there are any
     *
     * @brief MachineStates::afterSubMachinesStopped
     */
    void MachineStates::afterSubMachinesStopped()
    {
        // Tell the everyone the machine has finished
        if(error)
        {
            // Failed signal
            emit emit_machineFailed(errorDetails);
        }
        else
        {
            emit emit_machineFinished(errorDetails);
        }
    }


    /**
     * Shuts down the sub state machines shutdown machine without error
     *
     * @brief MachineStates::stopShutDownSubMachineWithoutError
     */
    void MachineStates::stopShutDownSubMachineWithoutError()
    {        
        // Stop the shutdown state machine
        shutDownMachine.stop();
    }

    /**
     * Shuts down the sub state machines shutdown machine with error
     *
     * @brief MachineStates::stopShutDownSubMachineWithError
     */
    void MachineStates::stopShutDownSubMachineWithError()
    {
        // Record there was an error
        error = true;

        // Stop the shutdown state machine
        shutDownMachine.stop();

        // Log error
        qCCritical(machineStates)    << childClassName           << " state machine stopped becuase of an error"   << "\n"
                                     << "Last pressure error: " << m_valves->errorDetails << "\n"
                                     << "Last guages error: "    << m_guages->errorDetails   << "\n"
                                     << "Last emergancy stop error: "    << m_emergancyStop->errorDetails   << "\n"
                                     << "Last pumps error: "    << m_pumps->errorDetails   << "\n"
                                     << "Last remote error: "    << m_remote->errorDetails   << "\n"
                                     << "Last temperature sensor error: "    << m_temperatureSensor->errorDetails;

    }


    /**
     * Emits a signal when the machine is stopped
     *
     * @brief MachineStates::emitStopped
     */
    void MachineStates::emitStopped()
    {
        // Run the stopped function in the state machine
        stopped();

        // Start the shut down machine if one present
        if(shutDownMachines && !shutDownMachine.isRunning())
        {
            // Build the shutdown machine
            buildShutDownMachine();

            // Run the sub machine shutdown state machine
            shutDownMachine.start();

            // Tell every one were stopping wuth the stop state machine
           emit emit_machineStopping(errorDetails);
        }

        // Stop main machine
        if(error && !shutDownMachines)
        {
            qDebug() << "Shutdown with failure";
            // Tell every we have stopped becuase of an error
            emit emit_machineFailed(errorDetails);
        }
        else if(!error && !shutDownMachines)
        {
            qDebug() << "Shutdown with success";
            // Tell every we have stopped becuase machine finished
            emit emit_machineFinished(errorDetails);
        }
    }


    /**
     * Stops and resets the machine with no errors flagged
     *
     * @brief MachineStates::stopMachineWithoutError
     */
    void MachineStates::stopMachineWithoutError()
    {
        // There was no error
        error = false;

        // Stop the machine
        if(machine.isRunning())
        {
            machine.stop();
        }
        else
        {
            emit emit_machineAlreadyStopped();
        }
    }


    /**
     * Stop and resets the machine with errors flagged
     *
     * @brief MachineStates::stopMachineWithError
     */
    void MachineStates::stopMachineWithError()
    {
        // There was an error
        error = true;

        // Log error
        qCCritical(machineStates)    << childClassName           << " state machine stopped becuase of an error"   << "\n"
                                     << "Last pressure error: " << m_valves->errorDetails << "\n"
                                     << "Last guages error: "    << m_guages->errorDetails   << "\n"
                                     << "Last emergancy stop error: "    << m_emergancyStop->errorDetails   << "\n"
                                     << "Last pumps error: "    << m_pumps->errorDetails   << "\n"
                                     << "Last remote error: "    << m_remote->errorDetails   << "\n"
                                     << "Last temperature sensor error: "    << m_temperatureSensor->errorDetails;

        // Stop the machine
        if(machine.isRunning())
        {
            machine.stop();
        }
        else
        {
            emit emit_machineAlreadyStopped();
        }
    }


    /**
     * This helper method removes all transistions from all the states
     *
     * @brief MachineStates::removeAllTransitions
     */
    void MachineStates::removeAllTransitions(QStateMachine &stateMachine)
    {
        // Get all states from machine and loop through them
        QList<QState *> allStates = stateMachine.findChildren<QState *>();
        while(!allStates.isEmpty())
        {
            // Get the current state
            QState *state = allStates.takeFirst();

            // Get all the transistions for this state
            QList<QAbstractTransition *> transitions = state->transitions();

            // Remove all the transisition from the states
            while (!transitions.isEmpty())
                state->removeTransition(transitions.takeFirst());
        }
    }


    /**
     * Allows override of params in state machine
     *
     * @brief MachineStates::paramsOverride
     */
    void MachineStates::paramsOverride(QVariantMap override)
    {
        for (auto i = override.constBegin(); i != override.constEnd(); ++i)
        {
             params.insert(i.key(), i.value());
        }
    }


    /**
     * Returns dynamically create states and creates them if they dont exists
     *
     * @brief MachineStates::state
     * @param id
     * @param machine
     * @return
     */
    QState* MachineStates::state(QString id, bool type)
    {
        // Append stop to stop state machine
        if(!type)
            id = "stop_" + id;

        // If does not exist then make it
        if(!m_states.contains(id))
        {
            m_states.insert(id, ( new QState( (type) ? &sm_master : &shutDownMachine ) ));
        }

        // return the state
        return m_states.value(id);
    }


    /**
     * Returns dynamically create validators and creates them if they dont exists
     *
     * @brief MachineStates::validator
     * @param id
     * @param machine
     * @return
     */
    CommandValidatorState* MachineStates::validator(QString id, bool type)
    {
        // Append stop to stop state machine
        if(!type)
            id = "stop_" + id;

        // If does not exist then make it
        if(!m_validators.contains(id))
        {
            m_validators.insert(id, ( new CommandValidatorState( (type) ? &sm_master : &shutDownMachine ) ));
        }

        // return the state
        return m_validators.value(id);
    }

}}}}
