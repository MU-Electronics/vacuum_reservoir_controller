#include "Global.h"

#include <fcntl.h>
#include <vector>


// Include external libs
#include <QMap>
#include <QDebug>
#include <QTimer>


using namespace QutiPi::Hardware::ADC;
using namespace QutiPi::Platform;

namespace App { namespace View { namespace Managers
{

    /**
     * Configure the view manager
     *
     * @brief Global::Global
     * @param parent
     * @param root
     * @param settings
     * @param experimentEngine
     */
    Global::Global(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)
    {

    }


    Global::~Global()
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
        // connect(&m_timer, &QTimer::timeout, this, &Global::togglePort);
        qDebug() << "Global make connections";
    }

    void Global::test()
    {
        qDebug() << "Hellow world";
    }

}}}

