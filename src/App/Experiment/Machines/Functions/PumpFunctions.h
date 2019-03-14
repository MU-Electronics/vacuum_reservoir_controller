#pragma once

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>

// Include functions helper
#include "../Helpers/FunctionsHelper.h"

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include command validator states
#include "../Helpers/CommandValidatorState.h"


namespace App { namespace Experiment { namespace Machines { namespace Functions
{
    class PumpFunctions : public QObject, public Helpers::FunctionsHelper
    {
        Q_OBJECT

        public:
            PumpFunctions(QObject* parent, Settings::Container *settings, Hardware::Access &hardware, QStateMachine& machine, QVariantMap& params, Hardware::CommandConstructor& commandConstructor);
            ~PumpFunctions();

            // Holds the application settings
            Settings::Container* m_settings;

            // Hold the hardware gateway
            Hardware::Access &m_hardware;

            // Hold the state machine
            QStateMachine &m_machine;

            // Hold params for states
            QVariantMap &m_params;

            // Hold instance of command constructor
            Hardware::CommandConstructor& m_commandConstructor;

            void connectStatesToMethods();

            // Error container
            QVariantMap errorDetails;

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);
            void emit_validationWrongId(QVariantMap data);

        public slots:
            void enablePump1();
            void enablePump2();

            void disablePump1();
            void disablePump2();

            void validateEnablePump1();
            void validateEnablePump2();

            void validateDisablePump1();
            void validateDisablePump2();

        private:
            void validatePumpHelper(int pumpId, bool state);


    };

}}}}
