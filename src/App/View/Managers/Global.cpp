#include "Global.h"

// Include external libs
#include <QMap>
#include <QDebug>

namespace App { namespace View { namespace Managers
{
    Global::Global(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings),

          // Define a digital pin
          examplePin(new DigitalIn(5))
    {

    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Global::makeConnections
     * @param hardware
     * @param safety
     */
    void Global::makeConnections()
    {
        qDebug() << examplePin.read();
    }


}}}

