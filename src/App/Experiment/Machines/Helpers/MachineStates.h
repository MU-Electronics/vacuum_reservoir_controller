#pragma once

#include <memory.h>

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QFinalState>
#include <QTimer>
#include <QVariantMap>

// Debugging catergories
#include "../../../Services/Debuging.h"

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include command validator states
#include "CommandValidatorState.h"

// Include functions


// Include transisiton builder
#include "TransitionsBuilder.h"

namespace App { namespace Experiment { namespace Machines { namespace Helpers
{
    class MachineStates : public QObject
    {
        Q_OBJECT
        public:
            MachineStates(QObject *parent, Settings::Container* settings, Hardware::Access &hardware);
            ~MachineStates();

            // Holds the application settings
            Settings::Container* m_settings;

            // Hold the hardware gateway
            Hardware::Access &m_hardware;

            // Hold instance of command constructor
            Hardware::CommandConstructor& m_commandConstructor;

            // Hold the state machine
            QStateMachine machine;

            // Hold params for states
            QVariantMap params;


            // Did an error occure which caused the machine to stop
            bool error = false;

            // Error cache for details on any error
            QVariantMap errorDetails;

            // Are there any sub state machines that requirer stopping
            bool shutDownMachines = false;

            // Hold the sub state machine shut down
            QStateMachine shutDownMachine;


            // Holds the dynamically created states
            QMap<QString, QState*> m_states;
            QMap<QString, CommandValidatorState*> m_validators;

            // Creates (if not exists) and returns the states
            QState *state(QString id, bool type);
            CommandValidatorState *validator(QString id, bool type);


            // External functions



            // State getters



            // Transition builder for common tasks
            TransitionsBuilder* m_transitionsBuilder;

            // Transnistion builder getter
            TransitionsBuilder* transitionsBuilder();


            QState sm_master;
            QFinalState sm_stop_2;

            // Create the stop and stopped as failed states
            QState
                sm_stop,
               sm_stopAsFailed

            ,   ssm_stop
            ,   ssm_stopAsFailed;


            // Start machine
            void start();


            // Cancel state machine
            void cancelStateMachine();


            // Helper methods
            void removeAllTransitions(QStateMachine &stateMachine);
            void paramsOverride(QVariantMap override);


            // Contract methods that must be implimented
            virtual void beforeStart(){}
            virtual void buildMachine() = 0;

            // Not all machines will have a stop procedure
            virtual void buildShutDownMachine(){}


            // Testing
            QString childClassName = "";
            // Connection helpers
            // void connectValve(QString name, bool machine, const QMetaMethod &method);

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);

            void emit_machineStopping(QVariantMap details);
            void emit_machineFinished(QVariantMap details);
            void emit_machineFailed(QVariantMap errorDetails);

            void emit_machineAlreadyStopped();

            void emit_cancelMachine();

        public slots:
            // Finishing state machine slots
            void stopMachineWithoutError();
            void stopMachineWithError();

            // Re-implimention of stopped for each machine
            virtual void stopped() = 0;

            // Allow re-implentation of machine finished signal emitter
            virtual void emitStopped();

            // Allow re-implentation of sub machine finished signal emitter
            virtual void afterSubMachinesStopped();

            // Stops sub state machine shutdown machine
            void stopShutDownSubMachineWithoutError();
            void stopShutDownSubMachineWithError();

        private:
            // Connect states to their function
            void connectStatesToMethods();

            // Holds status of inject machines
            QMap<QString, bool> m_injectedMachines;

    };
}}}}


