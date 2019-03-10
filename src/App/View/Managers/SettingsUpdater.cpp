#include "SettingsUpdater.h"


// Include external libs
#include <QDebug>
#include <QProcess>


namespace App { namespace View { namespace Managers
{

    /**
     * Configure the view manager
     *
     * @brief SettingsUpdater::SettingsUpdater
     * @param parent
     * @param root
     * @param settings
     * @param experimentEngine
     */
    SettingsUpdater::SettingsUpdater(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)

    {

    }


    SettingsUpdater::~SettingsUpdater()
    {

    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief SettingsUpdater::makeConnections
     * @param hardware
     * @param safety
     */
    void SettingsUpdater::makeConnections()
    {

    }


    void SettingsUpdater::updateBarrelSettings(int group, int autoState, int manualstate,
                              int alarmPressure, int alarmTime,
                              int lowerSetPoint, int upperSetPoint)
    {

    }


    void SettingsUpdater::updatePumpSettings(int group, int autoState, int manualstate,
                            int alarmPressure, int alarmTime,
                            int lowerSetPoint, int upperSetPoint, int warmupTime)
    {

    }


    void SettingsUpdater::updateCommentSettings(int group, QString comment)
    {

    }

}}}

