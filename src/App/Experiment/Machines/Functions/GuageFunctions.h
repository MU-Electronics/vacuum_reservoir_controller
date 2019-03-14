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
    class GuageFunctions : public QObject, public Helpers::FunctionsHelper
    {
        Q_OBJECT

        public:
            GuageFunctions(QObject* parent, Settings::Container *settings, Hardware::Access &hardware, QStateMachine& machine, QVariantMap& params, Hardware::CommandConstructor& commandConstructor);
            ~GuageFunctions();


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

            // Open valve states
            void readGroup1();
            void readGroup2();
            void readGroup3();
            void readGroup4();
            void readGroup5();
            void readGroup6();
            void readGroup7();
            void readGroup8();

            // Validate open valve states
            void validateReadGroup1();
            void validateReadGroup2();
            void validateReadGroup3();
            void validateReadGroup4();
            void validateReadGroup5();
            void validateReadGroup6();
            void validateReadGroup7();
            void validateReadGroup8();

        private:
            // Read pressure helpers
            void guagePresureHelper(int group);
            void validateGuagePressureHelper(int group);

    };

}}}}
