#include "SystemValues.h"


// Include external libs
#include <QDebug>


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
    SystemValues::SystemValues(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)
    {
        // Show initialising screen
        initialising(false); // << false for debug

        // Control status
        m_control.insert("remote", false);
        m_control.insert("manual_auto", false);
        m_control.insert("touch_control", false);
        emit_controlChanged(m_control);

        // Valve values
        m_valve.insert("1_status", int(Statuses::state2)); // Statuses::disabled
        m_valve.insert("1_name", "V1");
        m_valve.insert("2_status", int(Statuses::disabled));
        m_valve.insert("2_name", "V2");
        m_valve.insert("3_status", int(Statuses::disabled));
        m_valve.insert("3_name", "V3");
        m_valve.insert("4_status", int(Statuses::disabled));
        m_valve.insert("4_name", "V4");
        m_valve.insert("5_status", int(Statuses::disabled));
        m_valve.insert("5_name", "V5");
        m_valve.insert("6_status", int(Statuses::disabled));
        m_valve.insert("6_name", "V6");
        m_valve.insert("7_status", int(Statuses::disabled));
        m_valve.insert("7_name", "V7");
        m_valve.insert("8_status", int(Statuses::disabled));
        m_valve.insert("8_name", "V8");
        emit_valveChanged(m_valve);

        // Barrel values
        m_barrel.insert("1_status", int(Statuses::disabled));
        m_barrel.insert("1_name", "B1");
        m_barrel.insert("2_status", int(Statuses::disabled));
        m_barrel.insert("2_name", "B2");
        m_barrel.insert("3_status", int(Statuses::disabled));
        m_barrel.insert("3_name", "B3");
        m_barrel.insert("4_status", int(Statuses::disabled));
        m_barrel.insert("4_name", "B4");
        m_barrel.insert("5_status", int(Statuses::disabled));
        m_barrel.insert("5_name", "B5");
        m_barrel.insert("6_status", int(Statuses::disabled));
        m_barrel.insert("6_name", "B6");
        emit_barrelChanged(m_barrel);

        // Pressure values
        m_pressure.insert("1_status", int(Statuses::state2));
        m_pressure.insert("1_name", "P1");
        m_pressure.insert("1_value", "0");
        m_pressure.insert("2_status", int(Statuses::disabled));
        m_pressure.insert("2_name", "P2");
        m_pressure.insert("2_value", "0");
        m_pressure.insert("3_status", int(Statuses::disabled));
        m_pressure.insert("3_name", "P3");
        m_pressure.insert("3_value", "0");
        m_pressure.insert("4_status", int(Statuses::disabled));
        m_pressure.insert("4_name", "P4");
        m_pressure.insert("4_value", "0");
        m_pressure.insert("5_status", int(Statuses::disabled));
        m_pressure.insert("5_name", "P5");
        m_pressure.insert("5_value", "0");
        m_pressure.insert("6_status", int(Statuses::disabled));
        m_pressure.insert("6_name", "P6");
        m_pressure.insert("6_value", "0");
        m_pressure.insert("7_status", int(Statuses::disabled));
        m_pressure.insert("7_name", "P7");
        m_pressure.insert("7_value", "0");
        m_pressure.insert("8_status", int(Statuses::disabled));
        m_pressure.insert("8_name", "P8");
        m_pressure.insert("8_value", "0");
        emit_pressureChanged(m_pressure);

        // Comment values
        m_comment.insert("1_value", "");
        m_comment.insert("2_value", "");
        m_comment.insert("3_value", "");
        m_comment.insert("4_value", "");
        m_comment.insert("5_value", "");
        m_comment.insert("6_value", "");
        emit_commentChanged(m_comment);

        // Pumo values
        m_pump.insert("1_status", int(Statuses::disabled));
        m_pump.insert("1_name", "Pump 1");
        m_pump.insert("2_status", int(Statuses::disabled));
        m_pump.insert("2_name", "Pump 2");
        emit_pumpChanged(m_pump);
    }


    SystemValues::~SystemValues()
    {

    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Global::makeConnections
     * @param hardware
     * @param safety
     */
    void SystemValues::makeConnections()
    {

    }


    /**
     * Logic on whether the settings panel should show
     *
     * @brief SystemValues::showSettings
     * @return
     */
    bool SystemValues::showSettings()
    {
        // Only show when in manual mode with no touch control enabled
        if(m_control["manual_auto"] == false && m_control["touch_control"] == false && m_control["remote"] == false)
            return true;

        return false;
    }


    /**
     * When touch control should be allowed
     *
     * @TODO check settings to see if group disabled
     *
     * @brief SystemValues::allowTouchControl
     * @return
     */
    bool SystemValues::allowTouchControl(int group)
    {
        // Only allow when touch control enabled
        if(m_control["manual_auto"] == false && m_control["touch_control"] == true && m_control["remote"] == false)
            return true;

        return false;
    }


    /**
     * Set the state of control
     *
     * @brief SystemValues::setControlState
     * @param type
     * @param state
     */
    void SystemValues::setControlState(QString type, bool state)
    {
        // Set control
        m_control[type] = state;

        // Tell everyone we've updated
        emit_controlChanged(m_control);
    }

}}}

