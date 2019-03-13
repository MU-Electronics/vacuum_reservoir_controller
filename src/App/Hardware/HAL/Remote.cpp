#include "Remote.h"

#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QThread>
#include <QTextStream>


namespace App { namespace Hardware { namespace HAL
{
    Remote::Remote(QObject *parent, PinName remote)
        :    HALSupport(parent)

            // Set GPIO for pums
        ,   m_remote(*new QutiPi::Drivers::DigitalIn(remote))

    {
        setup();
    }


    void Remote::setup()
    {

    }


    void Remote::isConnected()
    {
        // Enable
        QString pressed = "1";

        // Read pin
        pressed = QString::number(m_remote.read());

        // Create simple package
        QStringList data;
        data.append(pressed);

        // Emit the data
        emit emit_remoteData("Remote", "isConnected", m_command, data);
    }

}}}
