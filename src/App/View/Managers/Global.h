#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>
#include <QMap>
#include <QVariantMap>
#include <QTimer>

#include <Hal/gpio_api.h>
#include <Hal/gpio_irq_api.h>
#include <Hal/pin_map.h>
#include <Hal/delay_api.h>
#include <Hal/time_api.h>

#include <Drivers/DigitalIn.h>
#include <Drivers/DigitalOut.h>
#include "Drivers/InterruptIn.h"

#include <Hardware/ADC/MCP3424.h>


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

            // Interrupt function
            void handlerInter();

        signals:

        public slots:
            void readAdc();
            void togglePort();
            void testInterrupt();

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Holds the connection statuses for all the hardware
            QVariantMap m_hardwareConnection;

            // Example digital pin input
            QutiPi::Drivers::DigitalIn& m_inputPin1;

            // MCP3424 control
            QutiPi::Hardware::ADC::MCP3424::Device m_device;
            QutiPi::Hardware::ADC::MCP3424* m_mcp3224;

            // Interrupt pin
            QutiPi::Drivers::InterruptIn m_interruptIn;

            // Current port state
            int m_portState = 1;
            int m_portState2 = 0;
            unsigned int interruptTime = 0;
            std::vector<unsigned int> interruptAverage;

            // Digital out pin
            QutiPi::Drivers::DigitalOut& m_outputPin1;
            QutiPi::Drivers::DigitalOut& m_outputPin2;
            QutiPi::Drivers::DigitalOut& m_outputPin3;
            QutiPi::Drivers::DigitalOut& m_outputPin4;
            QutiPi::Drivers::DigitalOut& m_outputPin5;

            // Timer
            QTimer &m_timer;

            void setupMcp3424();
            void setupInterrupts();

    };
}}}


