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
#include "ReadGuageTrip.h"
#include "PumpControl.h"

namespace App { namespace Experiment { namespace Machines
{
    class AutomaticControl :   public Helpers::MachineStates
    {
        Q_OBJECT

        public:
            AutomaticControl(QObject *parent, Settings::Container* settings, Hardware::Access &hardware, ReadGuageTrip &trips, PumpControl &pumpController);

            ~AutomaticControl();

            void setParams();

            void beforeStart();

            void stopped();

            void buildMachine();

        signals:
            void emit_usePump1();
            void emit_usePump2();
            void emit_noPumpsAvailable();

            void emit_pumpMarkedAsFailed(int id);

        public slots:
            void guageTripped(int group, bool state);
            void guagePressures(QVariantMap package);

            void selectPump();

            void requestPump1();
            void requestPump2();

            void markPumpFailure();

            void selectBarrel();

        private:
            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Pump controller
            PumpControl& m_pumpController;

            // Barrel that have been tripped
            QList<int> m_tripped;

            // Barrels that have been marked as leaking
            QList<int> m_leaked;

            // Barrals that have been disabled by usPumpControler
            QList<int> m_disabled;

            // Pressures
            QMap<int, double> m_pressures;

            // Mark pumps are usable
            int m_currentPump = 1;
            int m_disabledPump1 = 0;
            int m_disabledPump2 = 0;

    };
}}}


