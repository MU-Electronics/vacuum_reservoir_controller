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

// QutiPi drivers
#include <Drivers/InterruptIn.h>

using namespace QutiPi::Drivers;

namespace App { namespace View { namespace Managers
{

    class Power : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        // States for qml

        public:
            Power(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings);

            ~Power();

            // Make connections with outside world
            void makeConnections();

            // States

        signals:


        public slots:


        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Interrupts
            InterruptIn m_mainPowerDrop;
            InterruptIn m_shutDown;

            void setupInterrupts();

            void shutDown();
            void mainPowerDrop();

    };
}}}


