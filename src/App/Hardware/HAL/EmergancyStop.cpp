#include "EmergancyStop.h"

#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QThread>
#include <QTextStream>


namespace App { namespace Hardware { namespace HAL
{
    EmergancyStop::EmergancyStop(QObject *parent, PinName emergancyStop)
        :    HALSupport(parent)

            // Set GPIO for pums
        ,   m_emergancyStop(*new QutiPi::Drivers::DigitalIn(emergancyStop))

    {
        setup();
    }


    void EmergancyStop::setup()
    {

    }


    void EmergancyStop::isPressed()
    {
        // Enable
        QString pressed = "1";

        // Read pin
        pressed = QString::number(m_emergancyStop.read());

        // Create simple package
        QStringList data;
        data.append(pressed);

        // Emit the data
        emit emit_emergancyStopData("EmergancyStop", "isPressed", m_command, data);
    }

}}}
