#pragma once
#include <QObject>
#include <QSharedPointer>

// GPIO control
#include <Drivers/DigitalOut.h>


// Interface for HAL classes
#include "HALSupport.h"


namespace App { namespace Hardware { namespace HAL
{
    class Pumps: public HALSupport
    {
        Q_OBJECT

        public:
            Pumps(QObject *parent, PinName pump1, PinName pump2);

            void setup();

        signals:
            void emit_pumpData(QString responable, QString m_method, QVariantMap m_command, QStringList halData);

        public slots:
            void enable();
            void disable();

        private:

            // Voltage reading objects for board 1 & 2
            QutiPi::Drivers::DigitalOut m_pump_1;
            QutiPi::Drivers::DigitalOut m_pump_2;

    };

}}}
