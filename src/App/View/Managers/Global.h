#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>
#include <QTimer>

#include "Drivers/InterruptIn.h"
#include "Drivers/DigitalOut.h"
#include "Drivers/DigitalIn.h"

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
            Global(QObject *parent, QQmlApplicationEngine *root, Settings::Container *settings);

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
            Settings::Container* m_settings;

            // Timer
            QTimer &m_timer;

            // Guage board 1
            QutiPi::Hardware::ADC::MCP3424* m_guageADC1;
            QutiPi::Hardware::GPIO::MCP23008* m_guageGPIO1;

            // Guage board 2
            QutiPi::Hardware::ADC::MCP3424* m_guageADC2;
            QutiPi::Hardware::GPIO::MCP23008* m_guageGPIO2;

            // Pumps
            QutiPi::Drivers::DigitalOut m_pump1;
            QutiPi::Drivers::DigitalOut m_pump2;

            // Input remote
            QutiPi::Drivers::DigitalIn m_remote;

            // Valves
            QutiPi::Drivers::DigitalOut m_valve1;
            QutiPi::Drivers::DigitalOut m_valve2;
            QutiPi::Drivers::DigitalOut m_valve3;
            QutiPi::Drivers::DigitalOut m_valve4;
            QutiPi::Drivers::DigitalOut m_valve5;
            QutiPi::Drivers::DigitalOut m_valve6;
            QutiPi::Drivers::DigitalOut m_valve7;
            QutiPi::Drivers::DigitalOut m_valve8;

            // Guage tripped interrupts
            QutiPi::Drivers::InterruptIn m_guageTrip1;
            QutiPi::Drivers::InterruptIn m_guageTrip2;
            QutiPi::Drivers::InterruptIn m_guageTrip3;
            QutiPi::Drivers::InterruptIn m_guageTrip4;
            QutiPi::Drivers::InterruptIn m_guageTrip5;
            QutiPi::Drivers::InterruptIn m_guageTrip6;
            QutiPi::Drivers::InterruptIn m_guageTrip7;
            QutiPi::Drivers::InterruptIn m_guageTrip8;

    };
}}}


