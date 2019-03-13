#pragma once
#include <QObject>
#include <QSharedPointer>

// GPIO control
#include <Drivers/DigitalIn.h>


// Interface for HAL classes
#include "HALSupport.h"


namespace App { namespace Hardware { namespace HAL
{
    class Remote: public HALSupport
    {
        Q_OBJECT

        public:
            Remote(QObject *parent, PinName emergancyStop);

            void setup();

        signals:
            void emit_remoteData(QString responable, QString m_method, QVariantMap m_command, QStringList halData);

        public slots:
            void isConnected();

        private:

            // Emergancy stop button
            QutiPi::Drivers::DigitalIn m_remote;

    };

}}}
