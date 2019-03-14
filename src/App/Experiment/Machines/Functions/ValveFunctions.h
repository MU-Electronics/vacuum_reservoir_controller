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
    class ValveFunctions : public QObject, public Helpers::FunctionsHelper
    {
        Q_OBJECT

        public:
            ValveFunctions(QObject* parent, Settings::Container *settings, Hardware::Access &hardware, QStateMachine& machine, QVariantMap& params, Hardware::CommandConstructor& commandConstructor);
            ~ValveFunctions();


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
            void openGroup1();
            void openGroup2();
            void openGroup3();
            void openGroup4();
            void openGroup5();
            void openGroup6();
            void openGroup7();
            void openGroup8();

            // Validate open valve states
            void validateOpenGroup1();
            void validateOpenGroup2();
            void validateOpenGroup3();
            void validateOpenGroup4();
            void validateOpenGroup5();
            void validateOpenGroup6();
            void validateOpenGroup7();
            void validateOpenGroup8();

            // Close valve states
            void closeGroup1();
            void closeGroup2();
            void closeGroup3();
            void closeGroup4();
            void closeGroup5();
            void closeGroup6();
            void closeGroup7();
            void closeGroup8();

            // Validate close valve states
            void validateCloseGroup1();
            void validateCloseGroup2();
            void validateCloseGroup3();
            void validateCloseGroup4();
            void validateCloseGroup5();
            void validateCloseGroup6();
            void validateCloseGroup7();
            void validateCloseGroup8();

        private:
            // Open valve helper
            void valveHelper(int group, bool state);
            void validateValveHelper(int group, bool state);

    };

}}}}
