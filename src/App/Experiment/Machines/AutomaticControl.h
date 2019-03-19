#pragma once

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"

// Include possable machine states
#include "Helpers/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    class AutomaticControl :   public Helpers::MachineStates
    {
        Q_OBJECT

        public:
            AutomaticControl(QObject *parent, Settings::Container* settings, Hardware::Access &hardware);

            ~AutomaticControl();

            void setParams();

            void beforeStart();

            void stopped();

            void buildMachine();

        signals:

        public slots:

        private:
            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

    };
}}}


