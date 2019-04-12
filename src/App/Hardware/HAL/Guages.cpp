#include "Guages.h"

#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QThread>
#include <QTextStream>


namespace App { namespace Hardware { namespace HAL
{
    Guages::Guages(QObject *parent,
                   QutiPi::Drivers::I2C::Device readings_1, QutiPi::Drivers::I2C::Device readings_2,
                   QutiPi::Drivers::I2C::Device active_1, QutiPi::Drivers::I2C::Device active_2,
                   QMap<QString, int> adcMapping_1, QMap<QString, int> adcMapping_2,
                   QMap<QString, PinName> trip_1, QMap<QString, PinName> trip_2)
        :    HALSupport(parent)

             // Create voltage reading objects for board 1 & 2
        ,    m_readings_1(*new QutiPi::Hardware::ADC::MCP3424(readings_1))
        ,    m_readings_2(*new QutiPi::Hardware::ADC::MCP3424(readings_2))

             // Create GPIO expander to show active guages objects
        ,    m_active_1(*new QutiPi::Hardware::GPIO::MCP23008(active_1))
        ,    m_active_2(*new QutiPi::Hardware::GPIO::MCP23008(active_2))

             // Create GPIO for detecting trip signals
        ,    m_trip_1_1(*new QutiPi::Drivers::DigitalIn(trip_1["1"]))
        ,    m_trip_2_1(*new QutiPi::Drivers::DigitalIn(trip_1["2"]))
        ,    m_trip_3_1(*new QutiPi::Drivers::DigitalIn(trip_1["3"]))
        ,    m_trip_4_1(*new QutiPi::Drivers::DigitalIn(trip_1["4"]))
        ,    m_trip_1_2(*new QutiPi::Drivers::DigitalIn(trip_2["1"]))
        ,    m_trip_2_2(*new QutiPi::Drivers::DigitalIn(trip_2["2"]))
        ,    m_trip_3_2(*new QutiPi::Drivers::DigitalIn(trip_2["3"]))
        ,    m_trip_4_2(*new QutiPi::Drivers::DigitalIn(trip_2["4"]))
    {
        m_adcMapping_1 = adcMapping_1;
        m_adcMapping_2 = adcMapping_2;

        // Configure object and device
        m_readings_1.configure(QutiPi::Hardware::ADC::MCP3424::Port::One, QutiPi::Hardware::ADC::MCP3424::Bitrate::Twelve, QutiPi::Hardware::ADC::MCP3424::Conversion::Continious, QutiPi::Hardware::ADC::MCP3424::Gain::One);
        m_readings_2.configure(QutiPi::Hardware::ADC::MCP3424::Port::One, QutiPi::Hardware::ADC::MCP3424::Bitrate::Twelve, QutiPi::Hardware::ADC::MCP3424::Conversion::Continious, QutiPi::Hardware::ADC::MCP3424::Gain::One);
        setup();
    }


    void Guages::setup()
    {
        // Ports that need config
        QList<QutiPi::Hardware::GPIO::MCP23008::Port> ports = {
            QutiPi::Hardware::GPIO::MCP23008::Port::One,
            QutiPi::Hardware::GPIO::MCP23008::Port::Two,
            QutiPi::Hardware::GPIO::MCP23008::Port::Three,
            QutiPi::Hardware::GPIO::MCP23008::Port::Four
        };

        // Chips that need config
        QList<QutiPi::Hardware::GPIO::MCP23008> ics = {
            m_active_1, m_active_2
        };

        // Config ics & ports
        for(auto ic : ics)
        {
            for(auto port : ports)
            {
                ic.setDirection(port, QutiPi::Hardware::GPIO::MCP23008::Direction::Output);
                thread()->usleep(50);
                ic.setPolarity(port, QutiPi::Hardware::GPIO::MCP23008::Polarity::Same);
                thread()->usleep(50);
                ic.setPullUp(port, QutiPi::Hardware::GPIO::MCP23008::PullUp::Disable);
                thread()->usleep(50);
                ic.write(port, QutiPi::Hardware::GPIO::MCP23008::State::Low);
                thread()->usleep(50);
            }
        }
    }


    /**
     * Enable the active LED on the RJ45 connector
     *
     * @brief Guages::enabled
     * @param package
     */
    QStringList Guages::enabled(QVariantMap commands)
    {
        // Get requested port
        switch(commands["guage_id"].toInt())
        {
            case 1:
                m_active_1.write(QutiPi::Hardware::GPIO::MCP23008::Port::One, QutiPi::Hardware::GPIO::MCP23008::State::High);
            break;
            case 2:
                m_active_1.write(QutiPi::Hardware::GPIO::MCP23008::Port::Two, QutiPi::Hardware::GPIO::MCP23008::State::High);
            break;
            case 3:
                m_active_1.write(QutiPi::Hardware::GPIO::MCP23008::Port::Three, QutiPi::Hardware::GPIO::MCP23008::State::High);
            break;
            case 4:
                m_active_1.write(QutiPi::Hardware::GPIO::MCP23008::Port::Four, QutiPi::Hardware::GPIO::MCP23008::State::High);
            break;
            case 5:
                m_active_2.write(QutiPi::Hardware::GPIO::MCP23008::Port::One, QutiPi::Hardware::GPIO::MCP23008::State::High);
            break;
            case 6:
                m_active_2.write(QutiPi::Hardware::GPIO::MCP23008::Port::Two, QutiPi::Hardware::GPIO::MCP23008::State::High);
            break;
            case 7:
                m_active_2.write(QutiPi::Hardware::GPIO::MCP23008::Port::Three, QutiPi::Hardware::GPIO::MCP23008::State::High);
            break;
            case 8:
                m_active_2.write(QutiPi::Hardware::GPIO::MCP23008::Port::Four, QutiPi::Hardware::GPIO::MCP23008::State::High);
            break;
        }

        // Read the voltage
        thread()->usleep(50);

        // Create simple package
        QStringList data;
        data.append(QString::number(1));
        return data;
        // Emit the data
        //emit emit_guageData("Guages", "enabled", commands, data);
    }


    /**
     * Disable the active LED on the RJ45 connector
     *
     * @brief Guages::enabled
     * @param package
     */
    QStringList Guages::disabled(QVariantMap commands)
    {
        // Get requested port
        switch(commands["guage_id"].toInt())
        {
            case 1:
                m_active_1.write(QutiPi::Hardware::GPIO::MCP23008::Port::One, QutiPi::Hardware::GPIO::MCP23008::State::Low);
            break;
            case 2:
                m_active_1.write(QutiPi::Hardware::GPIO::MCP23008::Port::Two, QutiPi::Hardware::GPIO::MCP23008::State::Low);
            break;
            case 3:
                m_active_1.write(QutiPi::Hardware::GPIO::MCP23008::Port::Three, QutiPi::Hardware::GPIO::MCP23008::State::Low);
            break;
            case 4:
                m_active_1.write(QutiPi::Hardware::GPIO::MCP23008::Port::Four, QutiPi::Hardware::GPIO::MCP23008::State::Low);
            break;
            case 5:
                m_active_2.write(QutiPi::Hardware::GPIO::MCP23008::Port::One, QutiPi::Hardware::GPIO::MCP23008::State::Low);
            break;
            case 6:
                m_active_2.write(QutiPi::Hardware::GPIO::MCP23008::Port::Two, QutiPi::Hardware::GPIO::MCP23008::State::Low);
            break;
            case 7:
                m_active_2.write(QutiPi::Hardware::GPIO::MCP23008::Port::Three, QutiPi::Hardware::GPIO::MCP23008::State::Low);
            break;
            case 8:
                m_active_2.write(QutiPi::Hardware::GPIO::MCP23008::Port::Four, QutiPi::Hardware::GPIO::MCP23008::State::Low);
            break;
        }

        // Read the voltage
        thread()->usleep(50);

        // Create simple package
        QStringList data;
        data.append(QString::number(0));
        return data;
        // Emit the data
        //emit emit_guageData("Guages", "disabled", commands, data);
    }


    /**
     * Read the vaccumm from the guage via onboard ADC
     *
     * @brief Guages::readVacuum
     * @param package
     */
    QStringList Guages::readVacuum(QVariantMap commands)
    {
        // Get requested port
        double voltage = 0;
        switch(commands["guage_id"].toInt())
        {
            case 1:
                voltage = m_readings_1.read(QutiPi::Hardware::ADC::MCP3424::Port::One, QutiPi::Hardware::ADC::MCP3424::Type::VoltageSigleEnded);
            break;
            case 2:
                voltage = m_readings_1.read(QutiPi::Hardware::ADC::MCP3424::Port::Two, QutiPi::Hardware::ADC::MCP3424::Type::VoltageSigleEnded);
            break;
            case 3:
                voltage = m_readings_1.read(QutiPi::Hardware::ADC::MCP3424::Port::Three, QutiPi::Hardware::ADC::MCP3424::Type::VoltageSigleEnded);
            break;
            case 4:
                voltage = m_readings_1.read(QutiPi::Hardware::ADC::MCP3424::Port::Four, QutiPi::Hardware::ADC::MCP3424::Type::VoltageSigleEnded);
            break;
            case 5:
                voltage = m_readings_2.read(QutiPi::Hardware::ADC::MCP3424::Port::One, QutiPi::Hardware::ADC::MCP3424::Type::VoltageSigleEnded);
            break;
            case 6:
                voltage = m_readings_2.read(QutiPi::Hardware::ADC::MCP3424::Port::Two, QutiPi::Hardware::ADC::MCP3424::Type::VoltageSigleEnded);
            break;
            case 7:
                voltage = m_readings_2.read(QutiPi::Hardware::ADC::MCP3424::Port::Three, QutiPi::Hardware::ADC::MCP3424::Type::VoltageSigleEnded);
            break;
            case 8:
                voltage = m_readings_2.read(QutiPi::Hardware::ADC::MCP3424::Port::Four, QutiPi::Hardware::ADC::MCP3424::Type::VoltageSigleEnded);
            break;
        }

        // Sleep thread
        // thread()->sleep(10);

        // Create simple package
        QStringList data;
        data.append(QString::number(voltage));

        //if(commands["guage_id"].toInt() == 1)
        //    qDebug() << "Reading vacuum: " << data << " Command: "<<commands;

        return data;
        // Emit the data
        // qDebug() << commands;
        //emit emit_guageData("Guages", "readVacuum", commands, data);
    }


    /**
     * Detect if the pressure sensor is tripped
     *
     * @brief Guages::readTrip
     * @param package
     */
    QStringList Guages::readTrip(QVariantMap commands)
    {
        // Get requested port
        int logic = 0;
        switch(commands["guage_id"].toInt())
        {
            case 1:
                logic = m_trip_1_1.read();
            break;
            case 2:
                logic = m_trip_2_1.read();
            break;
            case 3:
                logic = m_trip_3_1.read();
            break;
            case 4:
                logic = m_trip_4_1.read();
            break;
            case 5:
                logic = m_trip_1_2.read();
            break;
            case 6:
                logic = m_trip_2_2.read();
            break;
            case 7:
                logic = m_trip_3_2.read();
            break;
            case 8:
                logic = m_trip_4_2.read();
            break;
        }

        // Read the voltage
        //thread()->sleep(2);

        // Create simple package
        QStringList data;
        data.append(QString::number(logic));
        return data;
        // Emit the data
        //emit emit_guageData("Guages", "readTrip", commands, data);
    }


}}}
