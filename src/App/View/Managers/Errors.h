#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QVariantMap>
#include <QDebug>
#include <QMap>
#include <QList>

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

    class Errors : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        // States for qml

        // Error alter box settings
        Q_PROPERTY(QVariantMap visualErrorSettings READ visualErrorSettings NOTIFY emit_visualErrorSettingsChanged)

        // Debug messages
        Q_PROPERTY(QString debugMessages READ debugMessages NOTIFY emit_debugMessagesChanged)

        public:
            Errors(QObject *parent, QQmlApplicationEngine *root, Settings::Container *settings);

            ~Errors();

            // Make connections with outside world
            void makeConnections();

            // Return the data for the current / last alert
            QVariantMap visualErrorSettings() const { return m_visualErrorSettings; }

            // Return the data for the debug messages
            QString debugMessages();

        signals:
            void emit_debugMessagesChanged(QList<QMap<QString, QString>>);
            void emit_visualErrorSettingsChanged(QVariantMap settings);

        public slots:
            // Called when log gets a new addtion
            void logChanged(QMap<QString, QString> message);

            // Acknowledge visual error message on screen
            void acknowledgeError();

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container* m_settings;

            // Visaul errors
            QMap<QString, bool> m_visualErrors;

            // Current error
            QVariantMap m_visualErrorSettings;

            // Holds errors that have occured
            QList<QMap<QString, QString>> m_debugMessages;


    };
}}}


