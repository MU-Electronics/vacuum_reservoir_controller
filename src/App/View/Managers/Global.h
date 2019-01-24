#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>
#include <QTimer>

#include "Drivers/InterruptIn.h"

#include <Drivers/Exceptions/I2CError.h>

#include <Hardware/ADC/MCP3424.h>
#include <Hardware/GPIO/MCP23008.h>


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

    class Global : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        AUTO_PROPERTY(int, currentNavigation)

        public:
            Global(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings);

            ~Global();

            // Make connections with outside world
            void makeConnections();

            void guageTripTriggered();

        signals:

        public slots:
            void readGuage();
            void setGuageLED();

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Timer
            QTimer &m_timer;

            // MCP3424 control
            QutiPi::Hardware::ADC::MCP3424* m_mcp3224;

            // MCP23008 control
            QutiPi::Hardware::GPIO::MCP23008* m_mcp23008;

            // Guage tripped interrupt
            QutiPi::Drivers::InterruptIn m_guageTrip;

    };
}}}


