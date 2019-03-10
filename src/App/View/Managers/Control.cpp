#include "Control.h"


// Include external libs
#include <QDebug>
#include <QProcess>


namespace App { namespace View { namespace Managers
{

    /**
     * Configure the view manager
     *
     * @brief Control::Control
     * @param parent
     * @param root
     * @param settings
     * @param experimentEngine
     */
    Control::Control(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)

    {
    }


    Control::~Control()
    {

    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Control::makeConnections
     * @param hardware
     * @param safety
     */
    void Control::makeConnections()
    {
    }


    /**
     * Toggle the valve status
     *
     * @brief Control::toggleValve
     * @param group
     */
    void Control::setValve(int group, bool status)
    {

    }

    void Control::setPump(int group, bool status)
    {

    }




    void Control::enableAutoControl()
    {

    }

    void Control::disableAutoControl()
    {

    }
}}}

