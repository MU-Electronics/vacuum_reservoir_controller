#pragma once
#include <QObject>
#include <QSharedPointer>

// GPIO control
#include <Drivers/DigitalOut.h>


// Interface for HAL classes
#include "HALSupport.h"


namespace App { namespace Hardware { namespace HAL
{
    class Valves: public HALSupport
    {
        Q_OBJECT

        public:
            Valves(QObject *parent, PinName valve_1, PinName valve_2, PinName valve_3, PinName valve_4, PinName valve_5
                   , PinName valve_6, PinName valve_7, PinName valve_8);

            void setup();

        signals:
            void emit_valveData(QString responable, QString m_method, QVariantMap m_command, QStringList halData);

        public slots:
            void open();
            void close();

        private:

            // Valve control
            QutiPi::Drivers::DigitalOut m_valve_1;
            QutiPi::Drivers::DigitalOut m_valve_2;
            QutiPi::Drivers::DigitalOut m_valve_3;
            QutiPi::Drivers::DigitalOut m_valve_4;
            QutiPi::Drivers::DigitalOut m_valve_5;
            QutiPi::Drivers::DigitalOut m_valve_6;
            QutiPi::Drivers::DigitalOut m_valve_7;
            QutiPi::Drivers::DigitalOut m_valve_8;

    };

}}}
