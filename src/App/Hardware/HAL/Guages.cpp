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
        // qDebug() << "Guage Reading: " <<
    }


    /**
     * Enable the active LED on the RJ45 connector
     *
     * @brief Guages::enabled
     * @param package
     */
    void Guages::enabled()
    {

    }


    /**
     * Disable the active LED on the RJ45 connector
     *
     * @brief Guages::enabled
     * @param package
     */
    void Guages::disabled()
    {

    }


    /**
     * Read the vaccumm from the guage via onboard ADC
     *
     * @brief Guages::readVacuum
     * @param package
     */
    void Guages::readVacuum()
    {
        // Read the voltage
        auto voltage = m_readings_1.read(QutiPi::Hardware::ADC::MCP3424::Port::One, QutiPi::Hardware::ADC::MCP3424::Type::VoltageSigleEnded);

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

    }


}}}
