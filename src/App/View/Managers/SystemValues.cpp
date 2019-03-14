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
    SystemValues::SystemValues(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings)
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
        m_control.insert("emergancy_stop_triggered", false);
        emit_controlChanged(m_control);


        // Valve values
        m_valve.insert("1_status", int(ValveStatus::closed)); // Statuses::disabled
        m_valve.insert("1_name", "V1");
        m_valve.insert("2_status", int(ValveStatus::closed));
        m_valve.insert("2_name", "V2");
        m_valve.insert("3_status", int(ValveStatus::closed));
        m_valve.insert("3_name", "V3");
        m_valve.insert("4_status", int(ValveStatus::closed));
        m_valve.insert("4_name", "V4");
        m_valve.insert("5_status", int(ValveStatus::closed));
        m_valve.insert("5_name", "V5");
        m_valve.insert("6_status", int(ValveStatus::closed));
        m_valve.insert("6_name", "V6");
        m_valve.insert("7_status", int(ValveStatus::closed));
        m_valve.insert("7_name", "V7");
        m_valve.insert("8_status", int(ValveStatus::closed));
        m_valve.insert("8_name", "V8");
        emit_valveChanged(m_valve);

        // Barrel values
        m_barrel.insert("1_status", int(BarrelStatus::withinTolerance));
        m_barrel.insert("1_name", "B1");
        m_barrel.insert("2_status", int(BarrelStatus::withinTolerance));
        m_barrel.insert("2_name", "B2");
        m_barrel.insert("3_status", int(BarrelStatus::withinTolerance));
        m_barrel.insert("3_name", "B3");
        m_barrel.insert("4_status", int(BarrelStatus::withinTolerance));
        m_barrel.insert("4_name", "B4");
        m_barrel.insert("5_status", int(BarrelStatus::withinTolerance));
        m_barrel.insert("5_name", "B5");
        m_barrel.insert("6_status", int(BarrelStatus::withinTolerance));
        m_barrel.insert("6_name", "B6");
        emit_barrelChanged(m_barrel);

        // Pressure values
        m_pressure.insert("1_status", int(PressureStatus::ok));
        m_pressure.insert("1_name", "P1");
        m_pressure.insert("1_value", "0");
        m_pressure.insert("2_status", int(PressureStatus::ok));
        m_pressure.insert("2_name", "P2");
        m_pressure.insert("2_value", "0");
        m_pressure.insert("3_status", int(PressureStatus::ok));
        m_pressure.insert("3_name", "P3");
        m_pressure.insert("3_value", "0");
        m_pressure.insert("4_status", int(PressureStatus::ok));
        m_pressure.insert("4_name", "P4");
        m_pressure.insert("4_value", "0");
        m_pressure.insert("5_status", int(PressureStatus::ok));
        m_pressure.insert("5_name", "P5");
        m_pressure.insert("5_value", "0");
        m_pressure.insert("6_status", int(PressureStatus::ok));
        m_pressure.insert("6_name", "P6");
        m_pressure.insert("6_value", "0");
        m_pressure.insert("7_status", int(PressureStatus::ok));
        m_pressure.insert("7_name", "P7");
        m_pressure.insert("7_value", "0");
        m_pressure.insert("8_status", int(PressureStatus::ok));
        m_pressure.insert("8_name", "P8");
        m_pressure.insert("8_value", "0");
        emit_pressureChanged(m_pressure);

        // Pumo values
        m_pump.insert("1_status", int(PumpStatus::off));
        m_pump.insert("1_name", "Pump 1");
        m_pump.insert("2_status", int(PumpStatus::off));
        m_pump.insert("2_name", "Pump 2");
        emit_pumpChanged(m_pump);

        // Update values
        setGeneralSettingEnables();
        setGeneralSettingComments();
        setGeneralSettingParamters();
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
    void SystemValues::makeConnections(Hardware::Access& hardware)
    {
        // When the general settings object is updated, refresh the gui
        connect(m_settings->general().data(), &Settings::General::emit_saved, this, &SystemValues::refreshGeneralSettings);

        // Connect object signals to hardware slots and visa versa
        connect(this, &SystemValues::hardwareRequest, &hardware, &Hardware::Access::hardwareAccess);

        // Valve status
        connect(&hardware, &Hardware::Access::emit_valveOpened, this, &SystemValues::valveChange);
        connect(&hardware, &Hardware::Access::emit_valveClosed, this, &SystemValues::valveChange);

        // Pump status
        connect(&hardware, &Hardware::Access::emit_pumpEnabled, this, &SystemValues::pumpChange);
        connect(&hardware, &Hardware::Access::emit_pumpDisabled, this, &SystemValues::pumpChange);

        // Guage status
        connect(&hardware, &Hardware::Access::emit_guageReadVacuum, this, &SystemValues::pumpChange);

        // Barrell status

    }

    void SystemValues::guageReadingChanged(QVariantMap data)
    {
        m_pressure.insert(data["guage_id"].toString() + "_status", data["view_status"]);
        m_pressure.insert(data["guage_id"].toString() + "_value", data["pressure_mbar"]);
        emit_pressureChanged(m_pressure);
    }

    void SystemValues::pumpChange(QVariantMap data)
    {
        m_pump.insert(data["pump_id"].toString() + "_status", data["view_status"]);
        emit_pumpChanged(m_pump);
    }

    void SystemValues::valveChange(QVariantMap data)
    {
        m_valve.insert(data["group"].toString() + "_status", data["view_status"]);
        emit_valveChanged(m_valve);
    }


    /**
     * Refresh general settings in object and hence gui
     *
     * @brief SystemValues::refreshGeneralSettings
     */
    void SystemValues::refreshGeneralSettings()
    {
        setGeneralSettingEnables();
        setGeneralSettingComments();
        setGeneralSettingParamters();
    }


    /**
     * Update the views on current states of items
     *
     * @brief SystemValues::setGeneralSettingStates
     */
    void SystemValues::setStates()
    {

    }


    void SystemValues::setGeneralSettingComments()
    {
        // General settings object
        auto general = m_settings->general();
        auto generalChamber_1 = general->chamber(1);
        auto generalChamber_2 = general->chamber(2);
        auto generalChamber_3 = general->chamber(3);
        auto generalChamber_4 = general->chamber(4);
        auto generalChamber_5 = general->chamber(5);
        auto generalChamber_6 = general->chamber(6);

        // Comment values
        m_comment.insert("1_value", generalChamber_1["comment"]);
        m_comment.insert("2_value", generalChamber_2["comment"]);
        m_comment.insert("3_value", generalChamber_3["comment"]);
        m_comment.insert("4_value", generalChamber_4["comment"]);
        m_comment.insert("5_value", generalChamber_5["comment"]);
        m_comment.insert("6_value", generalChamber_6["comment"]);
        emit_commentChanged(m_comment);
    }


    /**
     * Update the views on paramters based off the config file
     *
     * @brief SystemValues::setGeneralSettingParamters
     */
    void SystemValues::setGeneralSettingParamters()
    {
        // General settings object
        auto general = m_settings->general();

        // Chamber params
        auto generalChamber_1 = general->chamber(1);
        auto generalChamber_2 = general->chamber(2);
        auto generalChamber_3 = general->chamber(3);
        auto generalChamber_4 = general->chamber(4);
        auto generalChamber_5 = general->chamber(5);
        auto generalChamber_6 = general->chamber(6);

        m_barrel.insert("1_lower_set_point", generalChamber_1["lower_set_point"]);
        m_barrel.insert("1_upper_set_point", generalChamber_1["upper_set_point"]);
        m_barrel.insert("1_alarm_pressure", generalChamber_1["alarm_pressure"]);
        m_barrel.insert("1_alarm_time", generalChamber_1["alarm_time"]);

        m_barrel.insert("2_lower_set_point", generalChamber_2["lower_set_point"]);
        m_barrel.insert("2_upper_set_point", generalChamber_2["upper_set_point"]);
        m_barrel.insert("2_alarm_pressure", generalChamber_2["alarm_pressure"]);
        m_barrel.insert("2_alarm_time", generalChamber_2["alarm_time"]);

        m_barrel.insert("3_lower_set_point", generalChamber_3["lower_set_point"]);
        m_barrel.insert("3_upper_set_point", generalChamber_3["upper_set_point"]);
        m_barrel.insert("3_alarm_pressure", generalChamber_3["alarm_pressure"]);
        m_barrel.insert("3_alarm_time", generalChamber_3["alarm_time"]);

        m_barrel.insert("4_lower_set_point", generalChamber_4["lower_set_point"]);
        m_barrel.insert("4_upper_set_point", generalChamber_4["upper_set_point"]);
        m_barrel.insert("4_alarm_pressure", generalChamber_4["alarm_pressure"]);
        m_barrel.insert("4_alarm_time", generalChamber_4["alarm_time"]);

        m_barrel.insert("5_lower_set_point", generalChamber_5["lower_set_point"]);
        m_barrel.insert("5_upper_set_point", generalChamber_5["upper_set_point"]);
        m_barrel.insert("5_alarm_pressure", generalChamber_5["alarm_pressure"]);
        m_barrel.insert("5_alarm_time", generalChamber_5["alarm_time"]);

        m_barrel.insert("6_lower_set_point", generalChamber_6["lower_set_point"]);
        m_barrel.insert("6_upper_set_point", generalChamber_6["upper_set_point"]);
        m_barrel.insert("6_alarm_pressure", generalChamber_6["alarm_pressure"]);
        m_barrel.insert("6_alarm_time", generalChamber_6["alarm_time"]);



        // Pump params
        auto generalPump_1 = general->pump(1);
        auto generalPump_2 = general->pump(1);

        m_pump.insert("1_lower_set_point", generalPump_1["lower_set_point"]);
        m_pump.insert("1_upper_set_point", generalPump_1["upper_set_point"]);
        m_pump.insert("1_warm_up", generalPump_1["warm_up"]);
        m_pump.insert("1_alarm_pressure", generalPump_1["alarm_pressure"]);
        m_pump.insert("1_alarm_time", generalPump_1["alarm_time"]);

        m_pump.insert("2_lower_set_point", generalPump_2["lower_set_point"]);
        m_pump.insert("2_upper_set_point", generalPump_2["upper_set_point"]);
        m_pump.insert("2_warm_up", generalPump_2["warm_up"]);
        m_pump.insert("2_alarm_pressure", generalPump_2["alarm_pressure"]);
        m_pump.insert("2_alarm_time", generalPump_2["alarm_time"]);


        // Update the interface
        emit_barrelChanged(m_barrel);
        emit_pumpChanged(m_pump);
    }


    /**
     * Update the views on what is enabled based off the config file
     *
     * @brief SystemValues::setGeneralSettingEnables
     */
    void SystemValues::setGeneralSettingEnables()
    {
        // General settings object
        auto general = m_settings->general();
        auto generalChamber_1 = general->chamber(1);
        auto generalChamber_2 = general->chamber(2);
        auto generalChamber_3 = general->chamber(3);
        auto generalChamber_4 = general->chamber(4);
        auto generalChamber_5 = general->chamber(5);
        auto generalChamber_6 = general->chamber(6);

        // System state
        QString enableKey = (m_control["manual_auto"] == false) ? "manual_control_enabled" : "auto_control_enabled" ;

        // Disable / Off values
        m_valve.insert("1_enabled", generalChamber_1[enableKey]);
        m_valve.insert("2_enabled", generalChamber_2[enableKey]);
        m_valve.insert("3_enabled", generalChamber_3[enableKey]);
        m_valve.insert("4_enabled", generalChamber_4[enableKey]);
        m_valve.insert("5_enabled", generalChamber_5[enableKey]);
        m_valve.insert("6_enabled", generalChamber_6[enableKey]);

        m_pressure.insert("1_enabled", generalChamber_1[enableKey]);
        m_pressure.insert("2_enabled", generalChamber_2[enableKey]);
        m_pressure.insert("3_enabled", generalChamber_3[enableKey]);
        m_pressure.insert("4_enabled", generalChamber_4[enableKey]);
        m_pressure.insert("5_enabled", generalChamber_5[enableKey]);
        m_pressure.insert("6_enabled", generalChamber_6[enableKey]);

        m_barrel.insert("1_enabled", generalChamber_1[enableKey]);
        m_barrel.insert("1_manual", generalChamber_1["manual_control_enabled"]);
        m_barrel.insert("1_auto", generalChamber_1["auto_control_enabled"]);

        m_barrel.insert("2_enabled", generalChamber_2[enableKey]);
        m_barrel.insert("2_manual", generalChamber_2["manual_control_enabled"]);
        m_barrel.insert("2_auto", generalChamber_2["auto_control_enabled"]);

        m_barrel.insert("3_enabled", generalChamber_3[enableKey]);
        m_barrel.insert("3_manual", generalChamber_3["manual_control_enabled"]);
        m_barrel.insert("3_auto", generalChamber_3["auto_control_enabled"]);

        m_barrel.insert("4_enabled", generalChamber_4[enableKey]);
        m_barrel.insert("4_manual", generalChamber_4["manual_control_enabled"]);
        m_barrel.insert("4_auto", generalChamber_4["auto_control_enabled"]);

        m_barrel.insert("5_enabled", generalChamber_5[enableKey]);
        m_barrel.insert("5_manual", generalChamber_5["manual_control_enabled"]);
        m_barrel.insert("5_auto", generalChamber_5["auto_control_enabled"]);

        m_barrel.insert("6_enabled", generalChamber_6[enableKey]);
        m_barrel.insert("6_manual", generalChamber_6["manual_control_enabled"]);
        m_barrel.insert("6_auto", generalChamber_6["auto_control_enabled"]);


        // Get Pump settings
        auto generalPump_1 = general->pump(1);
        auto generalPump_2 = general->pump(2);

        // Status of pump groups
         m_pump.insert("1_enabled", generalPump_1[enableKey]);
         m_pump.insert("1_auto", generalPump_1["auto_control_enabled"]);
         m_pump.insert("1_manual", generalPump_1["manual_control_enabled"]);

         m_pump.insert("2_enabled", generalPump_2[enableKey]);
         m_pump.insert("2_auto", generalPump_2["auto_control_enabled"]);
         m_pump.insert("2_manual", generalPump_2["manual_control_enabled"]);

         m_pressure.insert("7_enabled", generalPump_1[enableKey]);
         m_pressure.insert("8_enabled", generalPump_2[enableKey]);
         m_valve.insert("7_enabled", generalPump_1[enableKey]);
         m_valve.insert("8_enabled", generalPump_2[enableKey]);


        // Light LEDS to show guages that are enabled
         qDebug() << "Setting rear lights for " << enableKey;
        emit hardwareRequest(m_commands.guageSetState(1, generalChamber_1[enableKey].toBool()));
        emit hardwareRequest(m_commands.guageSetState(2, generalChamber_2[enableKey].toBool()));
        emit hardwareRequest(m_commands.guageSetState(3, generalChamber_3[enableKey].toBool()));
        emit hardwareRequest(m_commands.guageSetState(4, generalChamber_4[enableKey].toBool()));
        emit hardwareRequest(m_commands.guageSetState(5, generalChamber_5[enableKey].toBool()));
        emit hardwareRequest(m_commands.guageSetState(6, generalChamber_6[enableKey].toBool()));
        emit hardwareRequest(m_commands.guageSetState(7, generalPump_1[enableKey].toBool()));
        emit hardwareRequest(m_commands.guageSetState(8, generalPump_2[enableKey].toBool()));

        // Update the interface
        emit_pressureChanged(m_pressure);
        emit_barrelChanged(m_barrel);
        emit_valveChanged(m_valve);
        emit_pumpChanged(m_pump);
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
     * @brief SystemValues::allowTouchControl
     * @return
     */
    bool SystemValues::allowTouchControl(int group)
    {
        qDebug() << "touchcontrol: " << group;
        // System state
        QString enableKey = (m_control["manual_auto"] == false) ? "manual_control_enabled" : "auto_control_enabled" ;

        // Chambed group
        bool chamber = false;
        if(group < 7)
        {
            chamber = m_settings->general()->chamber(group)[enableKey].toBool();
        }
        else if(group >= 7) // Pump group
        {
            chamber = m_settings->general()->pump(group - 6)[enableKey].toBool();
        }

        // Only allow when touch control enabled
        if(m_control["manual_auto"] == false && m_control["touch_control"] == true && m_control["remote"] == false && chamber)
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

        // Update view
        setGeneralSettingEnables();

        // Tell everyone we've updated
        emit_controlChanged(m_control);
    }

}}}

