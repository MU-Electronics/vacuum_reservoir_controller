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
//        m_guageGPIO2->setDirection(MCP23008::Port::One, MCP23008::Direction::Output);
//        m_guageGPIO2->setPolarity(MCP23008::Port::One, MCP23008::Polarity::Same);
//        m_guageGPIO2->setPullUp(MCP23008::Port::One, MCP23008::PullUp::Disable);
//        m_guageGPIO2->write(MCP23008::Port::One, MCP23008::State::High);
    }


    /**
     * Enable the active LED on the RJ45 connector
     *
     * @brief Guages::enabled
     * @param package
     */
    void Guages::enabled()
    {
        // Get requested port
        QutiPi::Hardware::GPIO::MCP23008::Port port = QutiPi::Hardware::GPIO::MCP23008::Port::One;
        auto& board = m_active_1;
        switch(m_command["guage_id"].toInt())
        {
            case 1:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::One;
            break;
            case 2:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Two;
            break;
            case 3:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Three;
            break;
            case 4:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Four;
            break;
            case 5:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::One;
                board = m_active_2;
            break;
            case 6:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Two;
                board = m_active_2;
            break;
            case 7:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Three;
                board = m_active_2;
            break;
            case 8:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Four;
                board = m_active_2;
            break;
        }

        // Read the voltage
        board.write(port, QutiPi::Hardware::GPIO::MCP23008::State::High);

        // Create simple package
        QStringList data;
        data.append(QString::number(1));

        // Emit the data
        emit emit_guageData("Guages", "enabled", m_command, data);
    }


    /**
     * Disable the active LED on the RJ45 connector
     *
     * @brief Guages::enabled
     * @param package
     */
    void Guages::disabled()
    {
        // Get requested port
        QutiPi::Hardware::GPIO::MCP23008::Port port = QutiPi::Hardware::GPIO::MCP23008::Port::One;
        auto& board = m_active_1;
        switch(m_command["guage_id"].toInt())
        {
            case 1:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::One;
            break;
            case 2:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Two;
            break;
            case 3:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Three;
            break;
            case 4:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Four;
            break;
            case 5:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::One;
                board = m_active_2;
            break;
            case 6:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Two;
                board = m_active_2;
            break;
            case 7:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Three;
                board = m_active_2;
            break;
            case 8:
                port = QutiPi::Hardware::GPIO::MCP23008::Port::Four;
                board = m_active_2;
            break;
        }

        // Read the voltage
        board.write(port, QutiPi::Hardware::GPIO::MCP23008::State::Low);

        // Create simple package
        QStringList data;
        data.append(QString::number(0));

        // Emit the data
        emit emit_guageData("Guages", "disabled", m_command, data);
    }


    /**
     * Read the vaccumm from the guage via onboard ADC
     *
     * @brief Guages::readVacuum
     * @param package
     */
    void Guages::readVacuum()
    {
        // Get requested port
        QutiPi::Hardware::ADC::MCP3424::Port port = QutiPi::Hardware::ADC::MCP3424::Port::One;
        auto& board = m_readings_1;
        switch(m_command["guage_id"].toInt())
        {
            case 1:
                port = QutiPi::Hardware::ADC::MCP3424::Port::One;
            break;
            case 2:
                port = QutiPi::Hardware::ADC::MCP3424::Port::Two;
            break;
            case 3:
                port = QutiPi::Hardware::ADC::MCP3424::Port::Three;
            break;
            case 4:
                port = QutiPi::Hardware::ADC::MCP3424::Port::Four;
            break;
            case 5:
                port = QutiPi::Hardware::ADC::MCP3424::Port::One;
                board = m_readings_2;
            break;
            case 6:
                port = QutiPi::Hardware::ADC::MCP3424::Port::Two;
                board = m_readings_2;
            break;
            case 7:
                port = QutiPi::Hardware::ADC::MCP3424::Port::Three;
                board = m_readings_2;
            break;
            case 8:
                port = QutiPi::Hardware::ADC::MCP3424::Port::Four;
                board = m_readings_2;
            break;
        }

        // Read the voltage
        auto voltage = board.read(port, QutiPi::Hardware::ADC::MCP3424::Type::VoltageSigleEnded);

        // Create simple package
        QStringList data;
        data.append(QString::number(voltage));

        // Emit the data
        emit emit_guageData("Guages", "readVacuum", m_command, data);
    }


    /**
     * Detect if the pressure sensor is tripped
     *
     * @brief Guages::readTrip
     * @param package
     */
    void Guages::readTrip()
    {
        // Get requested port
        auto& trip = m_trip_1_1;
        switch(m_command["guage_id"].toInt())
        {
            case 1:
                trip = m_trip_1_1;
            break;
            case 2:
                trip = m_trip_2_1;
            break;
            case 3:
                trip = m_trip_3_1;
            break;
            case 4:
                trip = m_trip_4_1;
            break;
            case 5:
                trip = m_trip_1_2;
            break;
            case 6:
                trip = m_trip_2_2;
            break;
            case 7:
                trip = m_trip_3_2;
            break;
            case 8:
                trip = m_trip_4_2;
            break;
        }

        // Read the voltage
        int logic = trip.read();

        // Create simple package
        QStringList data;
        data.append(QString::number(logic));

        // Emit the data
        emit emit_guageData("Guages", "readTrip", m_command, data);
    }


}}}
