#pragma once
#include <QObject>
#include <QSharedPointer>

// GPIO control
#include <Drivers/DigitalIn.h>
#include <Drivers/I2C.h>

// Include I2C expections
#include <Drivers/Exceptions/I2CError.h>

// Include chip support
#include <Hardware/ADC/MCP3424.h>
#include <Hardware/GPIO/MCP23008.h>


// Interface for HAL classes
#include "HALSupport.h"


namespace App { namespace Hardware { namespace HAL
{
    class Guages: public HALSupport
    {
        Q_OBJECT

        public:
            Guages(QObject *parent,
                           QutiPi::Drivers::I2C::Device readings_1, QutiPi::Drivers::I2C::Device readings_2,
                           QutiPi::Drivers::I2C::Device active_1, QutiPi::Drivers::I2C::Device active_2,
                           QMap<QString, int> adcMapping_1, QMap<QString, int> adcMapping_2,
                           QMap<QString, PinName> trip_1, QMap<QString, PinName> trip_2);

            void setup();

        signals:
            void emit_guageData(QString responable, QString m_method, QVariantMap m_command, QStringList halData);

        public slots:
            QStringList enabled(QVariantMap commands);
            QStringList disabled(QVariantMap commands);
            QStringList readVacuum(QVariantMap commands);
            QStringList readTrip(QVariantMap commands);

        private:

            // Voltage reading objects for board 1 & 2
            QutiPi::Hardware::ADC::MCP3424 m_readings_1;
            QutiPi::Hardware::ADC::MCP3424 m_readings_2;

            // Pin mappings for ADC
            QMap<QString, int> m_adcMapping_1;
            QMap<QString, int> m_adcMapping_2;

            // GPIO expander to show active guages objects for board 1 & 2
            QutiPi::Hardware::GPIO::MCP23008 m_active_1;
            QutiPi::Hardware::GPIO::MCP23008 m_active_2;

            // GPIO for detecting trip signals
            QutiPi::Drivers::DigitalIn m_trip_1_1;
            QutiPi::Drivers::DigitalIn m_trip_2_1;
            QutiPi::Drivers::DigitalIn m_trip_3_1;
            QutiPi::Drivers::DigitalIn m_trip_4_1;
            QutiPi::Drivers::DigitalIn m_trip_1_2;
            QutiPi::Drivers::DigitalIn m_trip_2_2;
            QutiPi::Drivers::DigitalIn m_trip_3_2;
            QutiPi::Drivers::DigitalIn m_trip_4_2;

    };

}}}
