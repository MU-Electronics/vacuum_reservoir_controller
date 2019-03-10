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
        // Get chamber
        auto chamber = m_settings->general()->chamber(group);

        // Update object
        chamber["auto_control_enabled"] = bool(autoState);
        chamber["manual_control_enabled"] = bool(manualstate);
        chamber["lower_set_point"] = lowerSetPoint;
        chamber["upper_set_point"] = upperSetPoint;
        chamber["alarm_pressure"] = alarmPressure;
        chamber["alarm_time"] = alarmTime;

        // Save settings
        if(group == 1)
            m_settings->general()->save(Settings::General::Type::chamber_1, chamber);
        else if (group == 2)
            m_settings->general()->save(Settings::General::Type::chamber_2, chamber);
        else if (group == 3)
            m_settings->general()->save(Settings::General::Type::chamber_3, chamber);
        else if (group == 4)
            m_settings->general()->save(Settings::General::Type::chamber_4, chamber);
        else if (group == 5)
            m_settings->general()->save(Settings::General::Type::chamber_5, chamber);
        else if (group == 6)
            m_settings->general()->save(Settings::General::Type::chamber_6, chamber);
    }


    void SettingsUpdater::updatePumpSettings(int group, int autoState, int manualstate,
                            int alarmPressure, int alarmTime,
                            int lowerSetPoint, int upperSetPoint, int warmupTime)
    {
        // Get chamber
        auto pump = m_settings->general()->pump(group);

        // Update object
        pump["auto_control_enabled"] = bool(autoState);
        pump["manual_control_enabled"] = bool(manualstate);
        pump["lower_set_point"] = lowerSetPoint;
        pump["upper_set_point"] = upperSetPoint;
        pump["alarm_pressure"] = alarmPressure;
        pump["alarm_time"] = alarmTime;
        pump["warm_up"] = warmupTime;

        // Save settings
        if(group == 1)
            m_settings->general()->save(Settings::General::Type::pump_1, pump);
        else if (group == 2)
            m_settings->general()->save(Settings::General::Type::pump_2, pump);
    }


    void SettingsUpdater::updateCommentSettings(int group, QString comment)
    {
        // Get chamber
        auto chamber = m_settings->general()->chamber(group);

        // Update object
        chamber["comment"] = comment;

        // Save settings
        if(group == 1)
            m_settings->general()->save(Settings::General::Type::chamber_1, chamber);
        else if (group == 2)
            m_settings->general()->save(Settings::General::Type::chamber_2, chamber);
        else if (group == 3)
            m_settings->general()->save(Settings::General::Type::chamber_3, chamber);
        else if (group == 4)
            m_settings->general()->save(Settings::General::Type::chamber_4, chamber);
        else if (group == 5)
            m_settings->general()->save(Settings::General::Type::chamber_5, chamber);
        else if (group == 6)
            m_settings->general()->save(Settings::General::Type::chamber_6, chamber);
    }

}}}

