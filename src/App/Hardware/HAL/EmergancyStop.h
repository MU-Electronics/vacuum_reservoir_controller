#pragma once
#include <QObject>
#include <QSharedPointer>

// GPIO control
#include <Drivers/DigitalIn.h>


// Interface for HAL classes
#include "HALSupport.h"


namespace App { namespace Hardware { namespace HAL
{
    class EmergancyStop: public HALSupport
    {
        Q_OBJECT

        public:
            EmergancyStop(QObject *parent, PinName emergancyStop);

            void setup();

        signals:
            void emit_emergancyStopData(QString responable, QString m_method, QVariantMap m_command, QStringList halData);

        public slots:
            void isPressed();

        private:

            // Emergancy stop button
            QutiPi::Drivers::DigitalIn m_emergancyStop;

    };

}}}
