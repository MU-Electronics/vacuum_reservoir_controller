#include "Global.h"

#include <fcntl.h>
#include <vector>


// Include external libs
#include <QMap>
#include <QDebug>
#include <QTimer>

using namespace QutiPi::Hardware::ADC;
using namespace QutiPi::Hardware::GPIO;
using namespace QutiPi::Platform;
using namespace QutiPi::Drivers;

namespace App { namespace View { namespace Managers
{

    /**
     * Configure the view manager
     *
     * @brief Global::Global
     * @param parent
     * @param root
     * @param settings
     * @param experimentEngine
     */
    Global::Global(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)

        ,   m_timer(*new QTimer(this))

            // Guage trip inputs
        ,   m_guageTrip1(GPIO_19)
        ,   m_guageTrip2(GPIO_18)
        ,   m_guageTrip3(GPIO_17)
        ,   m_guageTrip4(GPIO_16)
        ,   m_guageTrip5(GPIO_14)
        ,   m_guageTrip6(GPIO_15)
        ,   m_guageTrip7(GPIO_21)
        ,   m_guageTrip8(GPIO_20)


            // Pump controls
        ,   m_pump1(GPIO_45, 0)
        ,   m_pump2(GPIO_44, 0)

            // Remote control detection
        ,   m_remote(GPIO_43, PullNone)

            // Valve controls
        ,   m_valve1(GPIO_42, PullDown)
        ,   m_valve2(GPIO_41, PullDown)
        ,   m_valve3(GPIO_40, PullDown)
        ,   m_valve4(GPIO_39, PullDown)
        ,   m_valve5(GPIO_38, PullDown)
        ,   m_valve6(GPIO_37, PullDown)
        ,   m_valve7(GPIO_28, PullDown)
        ,   m_valve8(GPIO_29, PullDown)

    {
        // Setup ADC on board one and two
        m_guageADC1 = new MCP3424("/dev/i2c-1", 0x6a); // Working
        m_guageADC2 = new MCP3424("/dev/i2c-1", 0x6c); // Working

        // Configure object and device
        m_guageADC1->configure(MCP3424::Port::One, MCP3424::Bitrate::Twelve, MCP3424::Conversion::Continious, MCP3424::Gain::One);
        m_guageADC2->configure(MCP3424::Port::One, MCP3424::Bitrate::Twelve, MCP3424::Conversion::Continious, MCP3424::Gain::One);


        // Setup GPIO expander on board one and two
        m_guageGPIO1 = new MCP23008("/dev/i2c-1", 0x21); // Working
        m_guageGPIO2 = new MCP23008("/dev/i2c-1", 0x20); // Working

        // Configure MCP23008 object
        m_guageGPIO2->setDirection(MCP23008::Port::One, MCP23008::Direction::Output);
        m_guageGPIO2->setPolarity(MCP23008::Port::One, MCP23008::Polarity::Same);
        m_guageGPIO2->setPullUp(MCP23008::Port::One, MCP23008::PullUp::Disable);
        m_guageGPIO2->write(MCP23008::Port::One, MCP23008::State::High);


        // Configure guage trip interrupt    // All working and tested
        m_guageTrip8.mode(PullNone);
        m_guageTrip8.both(callback(this, &Global::guageTripTriggered));


        // Pump states
        m_pump2.write(0); // Working
        m_pump1.write(1); // Working

        // Valve state
        m_valve1.write(0); // Working
        m_valve2.write(0); // Working
        m_valve3.write(0); // Working
        m_valve4.write(0); // Working
        m_valve5.write(0); // Working
        m_valve6.write(1); // Not working
        m_valve7.write(0); // Working
        m_valve8.write(0); // Working
    }


    Global::~Global()
    {

    }


    void Global::readGuage()
    {
        try
        {
            // Guage 1 voltage
            auto guage_1 = m_guageADC2->read(MCP3424::Port::One, MCP3424::Type::VoltageSigleEnded);

            // Guage 2 voltage
            auto guage_2 = m_guageADC2->read(MCP3424::Port::Two, MCP3424::Type::VoltageSigleEnded);

            // Guage 2 voltage
            auto guage_3 = m_guageADC2->read(MCP3424::Port::Three, MCP3424::Type::VoltageSigleEnded);

            // Guage 3 voltage
            auto guage_4 = m_guageADC2->read(MCP3424::Port::Four, MCP3424::Type::VoltageSigleEnded);

            // Values
            // qDebug() << "1: " << guage_1 << " 2: " << guage_2 << " 3: " << guage_3 << " 4: " << guage_4;
        }
        catch(Exceptions::I2CError& e)
        {
            qDebug() << e.what();
        }
    }


    void Global::setGuageLED()
    {
        try
        {
           //m_mcp23008->write(MCP23008::Port::Two, MCP23008::State::High);
        }
        catch(Exceptions::I2CError& e)
        {
            qDebug() << e.what();
        }

    }


    void Global::guageTripTriggered()
    {
        qDebug() << "Valve trip logic changed: " << m_guageTrip8.read();
    }



    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Global::makeConnections
     * @param hardware
     * @param safety
     */
    void Global::makeConnections()
    {
        // When timer elapses run
        connect(&m_timer, &QTimer::timeout, this, &Global::readGuage);
        connect(&m_timer, &QTimer::timeout, this, &Global::setGuageLED);

        // Start timer
        m_timer.start(5000);
    }

}}}

