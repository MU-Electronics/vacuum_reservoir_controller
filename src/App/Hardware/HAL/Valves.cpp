#include "Valves.h"

#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QThread>
#include <QTextStream>


namespace App { namespace Hardware { namespace HAL
{
    Valves::Valves(QObject *parent, PinName valve_1, PinName valve_2, PinName valve_3, PinName valve_4, PinName valve_5
                   , PinName valve_6, PinName valve_7, PinName valve_8)
        :    HALSupport(parent)

            // Set GPIO for pums
        ,   m_valve_1(*new QutiPi::Drivers::DigitalOut(valve_1, PullNone))
        ,   m_valve_2(*new QutiPi::Drivers::DigitalOut(valve_2, PullNone))
        ,   m_valve_3(*new QutiPi::Drivers::DigitalOut(valve_3, PullNone))
        ,   m_valve_4(*new QutiPi::Drivers::DigitalOut(valve_4, PullNone))
        ,   m_valve_5(*new QutiPi::Drivers::DigitalOut(valve_5, PullNone))
        ,   m_valve_6(*new QutiPi::Drivers::DigitalOut(valve_6, PullNone))
        ,   m_valve_7(*new QutiPi::Drivers::DigitalOut(valve_7, PullNone))
        ,   m_valve_8(*new QutiPi::Drivers::DigitalOut(valve_8, PullNone))

    {
        setup();
    }


    void Valves::setup()
    {

    }



    /**
     * Open the valve
     *
     * @brief Valves::open
     */
    void Valves::open()
    {
        // Status of valve
        QString state = "0";

        // Set GPIO
        switch(m_command["valve_id"].toInt())
        {
            case 1:
                m_valve_1.write(1);
                state = "1";
            break;
            case 2:
                m_valve_2.write(1);
                state = "1";
            break;
            case 3:
                m_valve_3.write(1);
                state = "1";
            break;
            case 4:
                m_valve_4.write(1);
                state = "1";
            break;
            case 5:
                m_valve_5.write(1);
                state = "1";
            break;
            case 6:
                m_valve_6.write(1);
                state = "1";
            break;
            case 7:
                m_valve_7.write(1);
                state = "1";
            break;
            case 8:
                m_valve_8.write(1);
                state = "1";
            break;
        }

        // Create simple package
        QStringList data;
        data.append(state);

        // Emit the data
        emit emit_valveData("Valves", "opened", m_command, data);
    }


    /**
     * Close a valve
     *
     * @brief Valves::close
     * @param package
     */
    void Valves::close()
    {
        // State of valve
        QString state = "0";

        // Set GPIO
        switch(m_command["valve_id"].toInt())
        {
            case 1:
                m_valve_1.write(0);
                state = "0";
            break;
            case 2:
                m_valve_2.write(0);
                state = "0";
            break;
            case 3:
                m_valve_3.write(0);
                state = "0";
            break;
            case 4:
                m_valve_4.write(0);
                state = "0";
            break;
            case 5:
                m_valve_5.write(0);
                state = "0";
            break;
            case 6:
                m_valve_6.write(0);
                state = "0";
            break;
            case 7:
                m_valve_7.write(0);
                state = "0";
            break;
            case 8:
                m_valve_8.write(0);
                state = "0";
            break;
        }

        // Create simple package
        QStringList data;
        data.append(state);

        // Emit the data
        emit emit_valveData("Valves", "closed", m_command, data);
    }




}}}
