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

// Sub state machines
#include "LeakDetection.h"

namespace App { namespace Experiment { namespace Machines
{
    class PumpControl :   public Helpers::MachineStates
    {
        Q_OBJECT

        public:
            PumpControl(QObject *parent, Settings::Container* settings, Hardware::Access &hardware);

            ~PumpControl();

            void setParams(int pump, QString mode = "auto_control_enabled");

            void beforeStart();

            void stopped();

            void buildMachine();

            void buildShutDownMachine();

        signals:
            void emit_usingPump1();
            void emit_usingPump2();

            void emit_vacuumSufficent();
            void emit_vaccumNotSufficient();
            void emit_wrongGuage();

            void emit_timerStarted();
            void emit_timeElapsed();
            void emit_timerRunning();

        public slots:
            void startWarmup();
            void startToValve();
            void startToBarrel();

            void selectPump();
            void isVacuumSufficent();
            void leakDetecter();
            void stopLeakDetecter();

            void isBarrelTimerRunning();
            void isValveTimerRunning();


        private:
            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer& t_warmup;
            QTimer& t_pumpVoid;
            QTimer& t_pumpManifoldVoid;

            // Leak detector
            LeakDetection& m_leakDetection;

            int m_pumpId;
    };
}}}


