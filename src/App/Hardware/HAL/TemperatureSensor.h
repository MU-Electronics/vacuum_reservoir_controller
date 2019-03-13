#pragma once
#include <QObject>
#include <QSharedPointer>

// I2C control
#include <Drivers/I2C.h>

// Include I2C expections
#include <Drivers/Exceptions/I2CError.h>

// Hardware

// Interface for HAL classes
#include "HALSupport.h"


namespace App { namespace Hardware { namespace HAL
{
    class TemperatureSensor: public HALSupport
    {
        Q_OBJECT

        public:
            TemperatureSensor(QObject *parent, QutiPi::Drivers::I2C::Device sensor);

            void setup();

        signals:
            void emit_temperatureSensorData(QString responable, QString m_method, QVariantMap m_command, QStringList halData);

        public slots:
            void getTemperature();

        private:

            // Temperature sensor instance

    };

}}}
