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

// Hardware thread
#include "../../Hardware/Access.h"

// Command constructor
#include "../../Hardware/CommandConstructor.h"

// QutiPi drivers
#include <Drivers/InterruptIn.h>

using namespace QutiPi::Drivers;

namespace App { namespace View { namespace Managers
{

    class Control : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        public:
            Control(QObject *parent, QQmlApplicationEngine *root, Settings::Container *settings);

            ~Control();

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware);


        signals:
            // Hardware thread propergation
            void hardwareRequest(QVariantMap command);

        public slots:
            void setValve(int group, bool status);
            void setPump(int group, bool status);

            void enableAutoControl();
            void disableAutoControl();

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container* m_settings;

            // Command construcor
            Hardware::CommandConstructor m_commands;


    };
}}}


