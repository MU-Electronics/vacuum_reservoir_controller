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

        public slots:
            void startWarmup();

            void selectPump();

        private:
            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer& t_warmup;

            int m_pumpId;
    };
}}}


