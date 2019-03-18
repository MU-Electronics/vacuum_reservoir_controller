#include "Pumps.h"

#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QThread>
#include <QTextStream>


namespace App { namespace Hardware { namespace HAL
{
    Pumps::Pumps(QObject *parent, PinName pump1, PinName pump2)
        :    HALSupport(parent)

            // Set GPIO for pums
        ,   m_pump_1(*new QutiPi::Drivers::DigitalOut(pump1))
        ,   m_pump_2(*new QutiPi::Drivers::DigitalOut(pump2))

    {
        qDebug() << "Pump 1" << static_cast<int>(pump1);
        qDebug() << "Pump 2" << static_cast<int>(pump2);

        m_pump_1.write(0);
        m_pump_2.write(0);
        setup();
    }


    void Pumps::setup()
    {

    }



    void Pumps::disable()
    {
        // Assum enabled
        QString enabled = "1";

        // Set GPIO
        if(m_command["pump_id"] == 1)
        {
             m_pump_1.write(0);
             enabled = "0";
        }
        else if(m_command["pump_id"] == 2)
        {
            m_pump_2.write(0);
            enabled = "0";
        }

        // Create simple package
        QStringList data;
        data.append(enabled);

        // Emit the data
        emit emit_pumpData("Pumps", "disabled", m_command, data);
    }


    /**
     * Enable a pump
     *
     * @brief Pumps::enable
     * @param package
     */
    void Pumps::enable()
    {
        // Assum disabled
        QString enabled = "0";

        // Set GPIO
        if(m_command["pump_id"] == 1)
        {
            qDebug() << "Pump 1 enabled";
             m_pump_1.write(1);
             enabled = "1";
        }
        else if(m_command["pump_id"] == 2)
        {
            m_pump_2.write(1);
            enabled = "1";
        }


        // Create simple package
        QStringList data;
        data.append(enabled);

        // Emit the data
        emit emit_pumpData("Pumps", "enabled", m_command, data);
    }




}}}
