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
                              double alarmPressure, int alarmTime,
                              double lowerSetPoint, double upperSetPoint,
                              int leakDetection, int leakPeriod, double leakFall)
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
        chamber["leak_detection"] = bool(leakDetection);
        chamber["leak_period"] = leakPeriod;
        chamber["leak_max"] = leakFall;
        chamber["pumping_time"] = chamber["pumping_time"];
        chamber["heavy_load"] = chamber["heavy_load"];

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
                            double alarmPressure, int alarmTime,
                            double lowerSetPoint, double upperSetPoint, int warmupTime,
                            int pumpToValve, int pumpToBarrel,
                            int leakDetection, int leakPeriod, double leakFall)
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
        pump["pump_void"] = pumpToValve;
        pump["pump_manifold_void"] = pumpToBarrel;
        pump["leak_detection"] = bool(leakDetection);
        pump["leak_period"] = leakPeriod;
        pump["leak_max"] = leakFall;

        qDebug() << pump;

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

