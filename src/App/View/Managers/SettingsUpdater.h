#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QVariantMap>

// View contract
#include "Manager.h"

// Macro for c++ => QML properties
#include "../../Utilities/Properties.h"

// Include settings container
#include "../../Settings/Container.h"

// Experiment Engine with state machine methods
#include "../../Experiment/Engine.h"


namespace App { namespace View { namespace Managers
{

    class SettingsUpdater : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        public:
            SettingsUpdater(QObject *parent, QQmlApplicationEngine *root, Settings::Container *settings);

            ~SettingsUpdater();

            // Make connections with outside world
            void makeConnections();


        signals:


        public slots:
            void updateBarrelSettings(int group, int autoState, int manualstate,
                                      int pumpingTime, double heavyLoad,
                                      double lowerSetPoint, double upperSetPoint,
                                      int leakDetection, int leakPeriod, double leakFall, int leakDelay, int pumpLeakDelay);
            void updatePumpSettings(int group, int autoState, int manualstate, int maniFoldLeakDelay,
                                    double lowerSetPoint, double upperSetPoint, int warmupTime, int pumpToValve, int pumpToBarrel, int leakDetection, int leakPeriod, double leakFall);
            void updateCommentSettings(int group, QString comment);
            void updateGeneralSettings(bool valveMode, bool defaultPump, bool ignoreTrips);

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container* m_settings;


    };
}}}


