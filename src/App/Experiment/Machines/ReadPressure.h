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
    class ReadPressure :   public Helpers::MachineStates
    {
        Q_OBJECT

        public:
            ReadPressure(QObject *parent, Settings::Container* settings, Hardware::Access &hardware);

            ~ReadPressure();

            void setParams();

            void beforeStart();

            void stopped();

            void buildMachine();

        signals:
            void emit_goTo_1();
            void emit_goTo_2();
            void emit_goTo_3();
            void emit_goTo_4();
            void emit_goTo_5();
            void emit_goTo_6();
            void emit_goTo_7();
            void emit_goTo_8();

        public slots:
            void startPressureMonitor();

        private:
            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer& t_pressureMonitor;

            // Current guage being read
            int m_guageId = 0;

            // Calculate which guage to read next
            void nextGuage();

    };
}}}


