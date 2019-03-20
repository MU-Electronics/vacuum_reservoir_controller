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
#include "PumpControl.h"
#include "AutomaticControl.h"
#include "LeakDetection.h"
#include "ValveControl.h"

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
            void startPumpsControl(int pump, QString mode);
            void stopPumpsControl();

            // Valve state machine control
            void startValvesControl(int valveId, bool state);
            void stopVavlesControl();

            // Leak state machine control
            void startLeakDetection(int period, int group);
            void stopLeakDetection();

            // Auto control state machine control
            void startAutoControl();
            void stopAutoControl();



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

            // Pump control
            void emit_pumpMachineStarted(QString mode);
            void emit_pumpMachineStopped(QVariantMap params);
            void emit_pumpMachineFailed(QVariantMap params);
            void emit_pumpMachineStopping(QVariantMap params);

            // Auto control
            void emit_autoMachineStarted(QString mode);
            void emit_autoMachineStopped(QVariantMap params);
            void emit_autoMachineFailed(QVariantMap params);
            void emit_autoMachineStopping(QVariantMap params);

            // Leak detection
            void emit_leakMachineStarted(QString mode);
            void emit_leakMachineStopped(QVariantMap params);
            void emit_leakMachineFailed(QVariantMap params);
            void emit_leakMachineStopping(QVariantMap params);

            // Valve control
            void emit_valveMachineStarted(QString mode);
            void emit_valveMachineStopped(QVariantMap params);
            void emit_valveMachineFailed(QVariantMap params);
            void emit_valveMachineStopping(QVariantMap params);

        private:
            // Holds the application settings
            Settings::Container* m_settings;

            // State machines
            ReadPressure& m_readPressure;
            ReadGuageTrip& m_readGuageTrip;
            PumpControl& m_pumpControl;
            AutomaticControl& m_autoControl;
            LeakDetection& m_leakDetection;
            ValveControl& m_valveControl;

    };
}}}


