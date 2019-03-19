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

            void setParams(int period, int group);

            void beforeStart();

            void stopped();

            void buildMachine();

        signals:


        public slots:
            void startLeakPeriod();

        private:
            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer& t_leakPeriod;

            int m_period;
            int m_group;
    };
}}}


