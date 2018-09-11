#include "Global.h"

#include <fcntl.h>
#include <vector>


// Include external libs
#include <QMap>
#include <QDebug>
#include <QTimer>


using namespace QutiPi::Hardware::ADC;
using namespace QutiPi::Platform;

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

            // Define timers
        ,   m_timer(*new QTimer(this))

            // Define digital interrupts
        ,   m_interruptIn(GPIO_36)

            // Define digital inputs
        ,   m_inputPin1(*new QutiPi::Drivers::DigitalIn(GPIO_37, PullDown))

            // Define digital outputs
        ,   m_outputPin1(*new QutiPi::Drivers::DigitalOut(GPIO_38, 1))
        ,   m_outputPin2(*new QutiPi::Drivers::DigitalOut(GPIO_39, 1))
        ,   m_outputPin3(*new QutiPi::Drivers::DigitalOut(GPIO_40, 1))
        ,   m_outputPin4(*new QutiPi::Drivers::DigitalOut(GPIO_41, 1))
        ,   m_outputPin5(*new QutiPi::Drivers::DigitalOut(GPIO_42, 0))
    {
        // Example time
        qDebug() << "Current time (seconds): " << current_time(S) <<
                    " Current time (milli seconds): " << current_time(MS) <<
                    " Current time (micro seconds): " << current_time(US);

        // Run the setup ADC method
        setupMcp3424();

        // Setup interrupt gpios
        setupInterrupts();

        // Example of using delays
        delay(1, US);
    }


    Global::~Global()
    {
        qDebug() << "Deleting Global";
    }


    /**
     * Setup interrupts
     *
     * @brief Global::setupInterrupts
     */
    void Global::setupInterrupts()
    {
        // Setup interrupt
        m_interruptIn.mode(PullNone);
        m_interruptIn.rise(callback(this, &Global::handlerInter));
    }



    /**
     * Handle interrupts
     *
     * @brief Global::handlerInter
     */
    void Global::handlerInter()
    {
        // Grab time
        unsigned int time = current_time(US);

        // Delay
        unsigned int delay = time - interruptTime;

        // Generate an average
        double average = 0;
        interruptAverage.push_back(delay);
        for (std::vector<unsigned int>::iterator it = interruptAverage.begin(); it != interruptAverage.end(); ++it)
            average += *it;
        average = average / interruptAverage.size();

        // Show the results
        qDebug() << "Interrupt at: " << interruptTime << " Recieved at:" << time << " Delay: " << delay << " Average: " << average;
    }


    /**
     * Setup the ADC
     *
     * @brief Global::setupMcp3424
     */
    void Global::setupMcp3424()
    {
        // Configure the device
        m_device.address = 0x6e;
        m_device.location = "/dev/i2c-1";

        // Create Mcp3424 object
        m_mcp3224 = new MCP3424(m_device);

        // Configure object and device
        m_mcp3224->configure(MCP3424::Port::One, MCP3424::Bitrate::Fourteen, MCP3424::Conversion::Continious, MCP3424::Gain::One);
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
        // When timer elapses run read ADC
        //connect(&m_timer, &QTimer::timeout, this, &Global::readAdc);

        // When timer elapses run toggle port
        connect(&m_timer, &QTimer::timeout, this, &Global::togglePort);

        // When timer elapses run test interrupt
        connect(&m_timer, &QTimer::timeout, this, &Global::testInterrupt);

        // Start timer
        m_timer.start(1000);
    }


    void Global::testInterrupt()
    {
        // Set a port from the state of another port through a function
        m_outputPin5 = m_portState2;

        // Toggle state
        m_portState2 = (m_portState2 == 1) ? 0 : 1 ;

        // Current time
        interruptTime = current_time(US);
    }


    /**
     * Read ADC value
     *
     * @brief Global::readAdc
     */
    void Global::readAdc()
    {
       qDebug() << "Qutipi Digital: " << m_mcp3224->read(MCP3424::Port::One, MCP3424::Type::Digital)
                << "Qutipi Voltage: " << m_mcp3224->read(MCP3424::Port::One, MCP3424::Type::VoltageSigleEnded);
    }


    /**
     * Toggle port
     *
     * @brief Global::togglePort
     */
    void Global::togglePort()
    {
        // Set a port through a operator
        m_outputPin1 = m_portState;

        // Set a port through a function
        m_outputPin3.write(m_portState);

        // Set a port from the state of another port through an operator
        m_outputPin2 = m_outputPin1;

        // Set a port from the state of another port through a function
        m_outputPin4.write(m_outputPin2);

        // Toggle state
        m_portState = (m_portState == 1) ? 0 : 1 ;
    }

}}}

