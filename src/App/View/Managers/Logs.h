#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QVariantMap>
#include <QVariantList>

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

    class Logs : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        AUTO_PROPERTY(int, currentLog)
        AUTO_PROPERTY(QVariantList, logList)
        AUTO_PROPERTY(QVariantList, logData)
        AUTO_PROPERTY(QString, viewingLog)

        public:
            Logs(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings);

            ~Logs();

            // Make connections with outside world
            void makeConnections();


        signals:


        public slots:
            void selectLog(QString file);

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            void getLogs();

    };
}}}


