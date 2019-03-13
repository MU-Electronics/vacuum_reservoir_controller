#include "TemperatureSensor.h"

#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QThread>
#include <QTextStream>


namespace App { namespace Hardware { namespace HAL
{
    TemperatureSensor::TemperatureSensor(QObject *parent, QutiPi::Drivers::I2C::Device sensor)
        :    HALSupport(parent)

    {
        setup();
    }


    void TemperatureSensor::setup()
    {

    }


    void TemperatureSensor::getTemperature()
    {
        // Enable
        QString temp = "1";

        // Get temperture

        // Create simple package
        QStringList data;
        data.append(temp);

        // Emit the data
        emit emit_temperatureSensorData("TemperatureSensor", "getTemperature", m_command, data);
    }

}}}
