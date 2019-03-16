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
        ,   m_commands(*new Hardware::CommandConstructor(parent))

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
    void Control::makeConnections(Hardware::Access &hardware)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &Control::hardwareRequest, &hardware, &Hardware::Access::hardwareAccess);
    }


    /**
     * Toggle the valve status
     *
     * @brief Control::toggleValve
     * @param group
     */
    void Control::setValve(int group, bool status)
    {
        if(status)
        {
            emit hardwareRequest(m_commands.valveOpen(group));
        }
        else
        {
            emit hardwareRequest(m_commands.valveClose(group));
        }
    }

    void Control::setPump(int group, bool status)
    {
        if(status)
        {
            emit hardwareRequest(m_commands.pumpEnable(group - 6));
        }
        else
        {
            emit hardwareRequest(m_commands.pumpDisable(group - 6));
        }
    }




    void Control::enableAutoControl()
    {

    }

    void Control::disableAutoControl()
    {

    }
}}}

