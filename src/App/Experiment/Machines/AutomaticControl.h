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
#include "LeakDetection.h"

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

            void emit_goTo_1();
            void emit_goTo_2();
            void emit_goTo_3();
            void emit_goTo_4();
            void emit_goTo_5();
            void emit_goTo_6();

            void emit_vacuumManifoldSufficent();
            void emit_vacuumManifoldNotSufficent();
            void emit_possiablePumpManifoldLeak();

            void emit_invalidPumpNumber();

            void emit_invalidBarrelNumber();

        public slots:
            void guageTripped(int group, bool state);
            void guagePressures(QVariantMap package);

            void selectPump();

            void requestPump1();
            void requestPump2();

            void markPumpFailure();

            void selectBarrel();

            void manifoldPressure();
            void startManifoldPumpTimer();
            void manifoldLeakDetection();

            void closePumpValve();
            void validateClosePumpValve();

            void openBarrelValve();
            void validateOpenBarrelValve();

            void startBarrelPumpTimer();

            void startBarrelLeakDetect();

        private:
            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer& t_pumpManifold;
            QTimer& t_pumpBarrel;

            // Pump controller
            PumpControl& m_pumpController;

            // Leak detection state machine for manifold
            LeakDetection& m_manifoldLeakDetection;

            // Barrel that have been tripped
            QList<int> m_tripped;

            // Barrels that have been marked as leaking
            QList<int> m_leaked;

            // Barrels that have been marked as having too heavy load
            QList<int> m_heavyLoad;

            // Barrals that have been enabled by PumpControler
            QList<int> m_enabled;

            // Pressures
            QMap<int, double> m_pressures;

            // Current barrel
            int m_currentBarrel = 0;

            // Keep record of last maifold pressure check (small leak detection)
            double m_manifoldLastPressure;

            // Mark pumps are usable
            int m_currentPump = 1;
            int m_disabledPump1 = 0;
            int m_disabledPump2 = 0;

            void generateChamberTransitions(int id);

    };
}}}


