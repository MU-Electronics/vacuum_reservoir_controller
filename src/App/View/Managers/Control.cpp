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
    Control::Control(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings, Experiment::Engine* experimentEngine)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)
        ,   m_commands(*new Hardware::CommandConstructor(parent))
        ,   m_experimentEngine(experimentEngine)

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


    /**m_experimentEngine
     * Toggle the valve status
     *
     * @brief Control::toggleValve
     * @param group
     */
    void Control::setValve(int group, bool status)
    {
        if(status)
        {
            m_experimentEngine->machines().startValvesControl(group, status);
            //emit hardwareRequest(m_commands.valveOpen(group));
        }
        else
        {
            m_experimentEngine->machines().startValvesControl(group, status);
            //emit hardwareRequest(m_commands.valveClose(group));
        }
    }

    void Control::setPump(int group, bool status)
    {
        if(status)
        {
            m_experimentEngine->machines().startPumpsControl(group - 6, "auto_control_enabled");
            //emit hardwareRequest(m_commands.pumpEnable(group - 6));
        }
        else
        {
            m_experimentEngine->machines().stopPumpsControl();
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

