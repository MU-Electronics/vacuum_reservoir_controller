#pragma once

// Include extenral deps
#include <QObject>

// Debugging catergories
#include "../../Services/Debuging.h"

// Include settings container
#include "../../Settings/Container.h"

// Include hardware thread
#include "../../Hardware/Access.h"

// Include state machines
#include "ReadPressure.h"

namespace App { namespace Experiment { namespace Machines
{
    class MachineContainer    :   public QObject
    {
        Q_OBJECT

        public:
            MachineContainer(QObject *parent, Settings::Container* settings, Hardware::Access &hardware);

            ~MachineContainer();

            MachineContainer(const MachineContainer&) = delete;

            void makeConnections();

        public slots:
            // Vacuum guage state machine control
            void startReadingVacuumGuages(QString mode = "manual_control_enabled");
            void stopReadVacuum();

            // Pump state machine control

            // Valve state machine control

            //

        signals:
            // Vacuum guage state machine signals
            void emit_vacuumMachineStarted(QString mode);
            void emit_vacuumMachineStopped(QVariantMap params);
            void emit_vacuumMachineFailed(QVariantMap params);
            void emit_vacuumMachineStopping(QVariantMap params);

        private:
            // Holds the application settings
            Settings::Container* m_settings;

            // State machines
            ReadPressure& m_readPressure;

    };
}}}


