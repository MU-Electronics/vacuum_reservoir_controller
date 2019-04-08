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
                              int pumpingTime, double heavyLoad,
                              double lowerSetPoint, double upperSetPoint,
                              int leakDetection, int leakPeriod, double leakFall,
                              int leakDelay, int pumpLeakDelay)
    {
        // Get chamber
        auto chamber = m_settings->general()->chamber(group);

        // Update object
        chamber["auto_control_enabled"] = bool(autoState);
        chamber["manual_control_enabled"] = bool(manualstate);
        chamber["lower_set_point"] = lowerSetPoint;
        chamber["upper_set_point"] = upperSetPoint;
        chamber["alarm_pressure"] = chamber["alarm_pressure"]; // No interface for these atm as they are not used
        chamber["alarm_time"] = chamber["alarm_time"]; // No interface for these atm as they are not used
        chamber["leak_detection"] = bool(leakDetection);
        chamber["leak_period"] = leakPeriod;
        chamber["leak_max"] = leakFall;
        chamber["pumping_time"] = pumpingTime * 1000;
        chamber["heavy_load"] = heavyLoad;
        chamber["leak_delay"] = leakDelay * 1000;
        chamber["barrel_delay"] = pumpLeakDelay * 1000;

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
                            int maniFoldLeakDelay,
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
        pump["alarm_pressure"] = pump["alarm_pressure"];  // No interface for these atm as they are not used
        pump["alarm_time"] = pump["alarm_time"];  // No interface for these atm as they are not used
        pump["warm_up"] = warmupTime;
        pump["pump_void"] = pumpToValve;
        pump["pump_manifold_void"] = pumpToBarrel;
        pump["leak_detection"] = bool(leakDetection);
        pump["leak_period"] = leakPeriod;
        pump["leak_max"] = leakFall;
        pump["manifold_delay"] = maniFoldLeakDelay * 1000;

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


    void SettingsUpdater::updateGeneralSettings(bool valveMode, bool defaultPump, bool ignoreTrips)
    {
        // Set default pump
        if(defaultPump)
        {
            m_settings->general()->save(Settings::General::Type::default_pump, 1);
        }
        else
        {
            m_settings->general()->save(Settings::General::Type::default_pump, 2);
        }

        // Set valve mode
        m_settings->general()->save(Settings::General::Type::manual_safety_valve, valveMode);

        // Ignore inital trips
        m_settings->general()->save(Settings::General::Type::ignore_trips, ignoreTrips);
    }

}}}

