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
    class LeakDetection :   public Helpers::MachineStates
    {
        Q_OBJECT

        public:
            LeakDetection(QObject *parent, Settings::Container* settings, Hardware::Access &hardware);

            ~LeakDetection();

            void setParams(int group, int period, double fall, int sample = 10, int initDelay=0);

            void beforeStart();

            void stopped();

            void buildMachine();

        signals:
            void emit_samplesReachedNoLeak();
            void emit_leakDetected();
            void emit_continue();
            void emit_leakCastingError();
            void emit_wrongGuage();

        public slots:
            void startLeakPeriod();
            void checkPressure();
            void startInitDelay();

        private:
            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer& t_leakPeriod;
            QTimer& t_initDelay;

            int m_count;
            double m_pressure;
    };
}}}


