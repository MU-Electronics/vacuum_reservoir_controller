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
    class ReadPressure    :   public Helpers::MachineStates
    {
        Q_OBJECT

        public:
            ReadPressure(QObject *parent, Settings::Container* settings, Hardware::Access &hardware);

            ~ReadPressure();

            void setParams(int pressureSensorTimeInter);

            void beforeStart();

            void stopped();

            void buildMachine();

        signals:

        public slots:
            void startPressureMonitor();

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer t_pressureMonitor;

    };
}}}


