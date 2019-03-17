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
#include "ReadGuageTrip.h"

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

            void startReadingTripVacuumGuages(QString mode = "manual_control_enabled");
            void stopReadTripVacuum();

            // Pump state machine control

            // Valve state machine control

            //

        signals:
            // Vacuum guage state machine signals
            void emit_vacuumMachineStarted(QString mode);
            void emit_vacuumMachineStopped(QVariantMap params);
            void emit_vacuumMachineFailed(QVariantMap params);
            void emit_vacuumMachineStopping(QVariantMap params);

            void emit_vacuumTripMachineStarted(QString mode);
            void emit_vacuumTripMachineStopped(QVariantMap params);
            void emit_vacuumTripMachineFailed(QVariantMap params);
            void emit_vacuumTripMachineStopping(QVariantMap params);
            void emit_guageTripped(int guage, bool state);
            void emit_guageTrippedFailed();

        private:
            // Holds the application settings
            Settings::Container* m_settings;

            // State machines
            ReadPressure& m_readPressure;
            ReadGuageTrip& m_readGuageTrip;

    };
}}}


