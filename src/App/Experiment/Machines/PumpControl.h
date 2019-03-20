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

        signals:
            void emit_usingPump1();
            void emit_usingPump2();

            void emit_vacuumSufficent();
            void emit_vaccumNotSufficient();

        public slots:
            void startWarmup();
            void selectPump();
            void isVacuumSufficent();
            void leakDetecter();
            void stopLeakDetecter();


        private:
            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer& t_warmup;

            // Leak detector
            LeakDetection& m_leakDetection;

            int m_pumpId;
    };
}}}


