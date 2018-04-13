#include "Global.h"


#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>


// Include external libs
#include <QMap>
#include <QDebug>
#include <QTimer>

#include <Hal/gpio.h>


namespace App { namespace View { namespace Managers
{
    Global::Global(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings),
          m_timer(*new QTimer(this)),

          // Define a digital pin
          examplePin(*new DigitalIn())
    {

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
        qDebug() << examplePin.read();

        qDebug() << "Access via main app: " << gpio_read();



        // Test ADC
        connect(&m_timer, &QTimer::timeout, this, &Global::readAdc);
        m_timer.start(5000);
    }


    void Global::readAdc()
    {
        // Create I2C bus
        int file;
        char *bus = "/dev/i2c-1";
        if((file = open(bus, O_RDWR)) < 0)
        {
            qDebug() << "Failed to open the bus.";

            return;
        }
        // Get I2C device, MCP3428 I2C address is 0x68(104)
        ioctl(file, I2C_SLAVE, 0x6e);

        // Select configuration command(0x10)
        // Continuous conversion mode, Channel-1, 12-bit resolution
        char config[1] = {0};
        config[0] = 0x10;
        write(file, config, 1);
        sleep(1);

        // Read 2 bytes of data from register(0x00)
        // raw_adc msb, raw_adc lsb
        char reg[1] = {0x00};
        write(file, reg, 1);
        char data[2] = {0};

        if(read(file, data, 2) != 2)
        {
            qDebug() << "Error : Input/Output error";
        }
        else
        {
            // Convert the data to 12-bits
            int raw_adc = ((data[0] & 0x0F) * 256 + data[1]);
            if(raw_adc > 2047)
            {
                raw_adc -= 4095;
            }

            // Output data to screen
            qDebug() << "Digital value of Analog Input : " << raw_adc;
        }


        m_timer.start(100);
    }


}}}

