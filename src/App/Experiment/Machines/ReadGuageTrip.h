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
    class ReadGuageTrip :   public Helpers::MachineStates
    {
        Q_OBJECT

        public:
            ReadGuageTrip(QObject *parent, Settings::Container* settings, Hardware::Access &hardware);

            ~ReadGuageTrip();

            void setParams(QString mode = "auto_control_enabled");

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

            void emit_guageTripped(int group, bool state);
            void emit_guageTrippedFailed();

        public slots:
            void startTripMonitor();
            void emitTripped();

        private:
            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer& t_tripMonitor;

            // Current guage being read
            int m_guageId = 0;

            // State of trips
            QMap<int, bool> m_trips;

            // Calculate which guage to read next
            void nextGuage();

            // What mode are we in?
            QString m_mode = "auto_control_enabled";

    };
}}}


