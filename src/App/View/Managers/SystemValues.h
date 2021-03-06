#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QVariantMap>
#include <QTimer>

// View contract
#include "Manager.h"

// Macro for c++ => QML properties
#include "../../Utilities/Properties.h"

// Include settings container
#include "../../Settings/Container.h"

// Experiment Engine with state machine methods
#include "../../Experiment/Engine.h"

// Hardware thread
#include "../../Hardware/Access.h"

// Command constructor
#include "../../Hardware/CommandConstructor.h"

// QutiPi drivers
#include <Drivers/InterruptIn.h>

namespace App { namespace View { namespace Managers
{

    class SystemValues : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        // System states for qml
        Q_PROPERTY(QVariantMap valveState READ valveState NOTIFY emit_valveChanged)
        Q_PROPERTY(QVariantMap barrelState READ barrelState NOTIFY emit_barrelChanged)
        Q_PROPERTY(QVariantMap pressureState READ pressureState NOTIFY emit_pressureChanged)
        Q_PROPERTY(QVariantMap commentSettings READ commentSettings NOTIFY emit_commentSettingsChanged)
        Q_PROPERTY(QVariantMap pumpState READ pumpState NOTIFY emit_pumpChanged)
        Q_PROPERTY(QVariantMap controlState READ controlState NOTIFY emit_controlChanged)
        Q_PROPERTY(QVariantMap pumpSettings READ pumpSettings NOTIFY emit_pumpSettingsChanged)
        Q_PROPERTY(QVariantMap barrelSettings READ barrelSettings NOTIFY emit_barrelSettingsChanged)
        AUTO_PROPERTY(bool, initialising)

        public:
            SystemValues(QObject *parent, QQmlApplicationEngine *root, Settings::Container *settings, Experiment::Engine *experimentEngine);

            ~SystemValues();

            // Make connections with outside world
            void makeConnections(Hardware::Access &hardware);

            // System states
            QVariantMap valveState() const { return m_valve; }
            QVariantMap barrelState() const { return m_barrel; }
            QVariantMap pressureState() const { return m_pressure; }
            QVariantMap pumpState() const { return m_pump; }
            QVariantMap controlState() const { return m_control; }
            QVariantMap pumpSettings() const { return m_pumpSettings; }
            QVariantMap barrelSettings() const { return m_barrelSettings; }
            QVariantMap commentSettings() const { return m_commentSettings; }


            void emergancyStopIntr();

        signals:
            // GUI signals
            void emit_valveChanged(QVariantMap);
            void emit_barrelChanged(QVariantMap);
            void emit_pressureChanged(QVariantMap);
            void emit_pumpChanged(QVariantMap);
            void emit_controlChanged(QVariantMap);
            void emit_pumpSettingsChanged(QVariantMap);
            void emit_barrelSettingsChanged(QVariantMap);
            void emit_commentSettingsChanged(QVariantMap);

            // Hardware thread propergation
            void hardwareRequest(QVariantMap command);

        public slots:
            bool showSettings();
            bool allowTouchControl(int group);
            void setControlState(QString type, bool state);
            void setStates();

            void refreshGeneralSettings();
            void setGeneralSettingEnables();
            void setGeneralSettingComments();
            void setGeneralSettingParamters();

            void valveChange(QVariantMap data);
            void pumpChange(QVariantMap data);
            void guageReadingChanged(QVariantMap data);
            void guageTripChanged(int group, bool state);

            void startVacuumGuages();
            void stopVacuumGuages();

            void stopVacuumTripGuages();
            void startVacuumTripGuages();

            void hardwardThreadStart();

            void autoControlFailed();
            void autoControlFinished();

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container* m_settings;

            // Hold experiment engine
            Experiment::Engine* m_experimentEngine;

            // Command construcor
            Hardware::CommandConstructor m_commands;

            // Holds all the control status data
            QVariantMap m_control;

            // Holds all the vavle status data
            QVariantMap m_valve;

            // Holds all the barrel status data
            QVariantMap m_barrel;
            QVariantMap m_barrelSettings;

            // Holds all the pressure sensor data
            QVariantMap m_pressure;

            // Holds all the pump data
            QVariantMap m_pump;
            QVariantMap m_pumpSettings;

            // Holds all comments
            QVariantMap m_commentSettings;

            // Status types
            enum class Statuses: int
            {
                state1 = 1,
                state2 = 2,
                state3 = 3,
                disabled = 4
            };

            // Valve states
            enum class ValveStatus: int
            {
                open = 1,
                closed = 2,
                error = 3,
                disabled = 0
            };

            // Pump states
            enum class PumpStatus: int
            {
                on = 1,
                off = 2,
                error = 3,
                disabled = 0
            };

            // Pressure states
            enum class PressureStatus: int
            {
                ok = 1,
                range = 2,
                error = 3,
                disabled = 0
            };

            // Barrel states
            enum class BarrelStatus: int
            {
                withinTolerance = 1,
                requiresToppingUp = 2,
                error = 3,
                disabled = 0
            };

            QString currentMode();
    };
}}}


