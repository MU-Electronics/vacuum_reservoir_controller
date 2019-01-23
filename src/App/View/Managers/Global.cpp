#include "Global.h"

#include <fcntl.h>
#include <vector>


// Include external libs
#include <QMap>
#include <QDebug>
#include <QTimer>

using namespace QutiPi::Hardware::ADC;
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
    {
        // Create Mcp3424 object 2: 0x6a
        m_mcp3224 = new MCP3424("/dev/i2c-1", 0x6c);

        // Configure object and device
        m_mcp3224->configure(MCP3424::Port::One, MCP3424::Bitrate::Twelve, MCP3424::Conversion::Continious, MCP3424::Gain::One);
    }


    Global::~Global()
    {

    }


    void Global::readGuage()
    {
        try
        {
            // Guage 1 voltage
            auto guage_1 = m_mcp3224->read(MCP3424::Port::One, MCP3424::Type::VoltageSigleEnded);

            // Guage 2 voltage
            auto guage_2 = m_mcp3224->read(MCP3424::Port::Two, MCP3424::Type::VoltageSigleEnded);

            // Guage 2 voltage
            auto guage_3 = m_mcp3224->read(MCP3424::Port::Three, MCP3424::Type::VoltageSigleEnded);

            // Guage 3 voltage
            auto guage_4 = m_mcp3224->read(MCP3424::Port::Four, MCP3424::Type::VoltageSigleEnded);


            // Values
            qDebug() << "1: " << guage_1 << " 2: " << guage_2 << " 3: " << guage_3 << " 4: " << guage_4;
        }
        //catch(...)
        catch(Exceptions::I2CError& e)
        {
            qDebug() << e.what();
        }
    }


    void Global::setGuageLED()
    {

    }


    void Global::detectGuageTrip()
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
        // When timer elapses run toggle port
        connect(&m_timer, &QTimer::timeout, this, &Global::readGuage);

       // Start timer
       m_timer.start(1000);
    }

}}}

