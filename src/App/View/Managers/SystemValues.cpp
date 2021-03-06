#include "SystemValues.h"

#include <math.h>

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
    SystemValues::SystemValues(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings, Experiment::Engine* experimentEngine)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)
        ,   m_experimentEngine(experimentEngine)
    {
        // Show initialising screen
        initialising(true);

        // Wait awhile to fill moving averages
        QTimer::singleShot(10000, [this]()
        {
            initialising(false);
        });


        // Control status
        m_control.insert("remote", false);
        m_control.insert("manual_auto", false);
        m_control.insert("touch_control", false);
        m_control.insert("emergancy_stop_triggered", false);
        emit_controlChanged(m_control);


        // Valve values
        m_valve.insert("1_status", int(ValveStatus::closed)); // Statuses::disabled
        m_valve.insert("1_name", "V1");
        m_valve.insert("1_trip", 0);
        m_valve.insert("2_status", int(ValveStatus::closed));
        m_valve.insert("2_name", "V2");
        m_valve.insert("2_trip", 0);
        m_valve.insert("3_status", int(ValveStatus::closed));
        m_valve.insert("3_name", "V3");
        m_valve.insert("3_trip", 0);
        m_valve.insert("4_status", int(ValveStatus::closed));
        m_valve.insert("4_name", "V4");
        m_valve.insert("4_trip", 0);
        m_valve.insert("5_status", int(ValveStatus::closed));
        m_valve.insert("5_name", "V5");
        m_valve.insert("5_trip", 0);
        m_valve.insert("6_status", int(ValveStatus::closed));
        m_valve.insert("6_name", "V6");
        m_valve.insert("6_trip", 0);
        m_valve.insert("7_status", int(ValveStatus::closed));
        m_valve.insert("7_name", "V7");
        m_valve.insert("7_trip", 0);
        m_valve.insert("8_status", int(ValveStatus::closed));
        m_valve.insert("8_name", "V8");
        m_valve.insert("8_trip", 0);
        emit_valveChanged(m_valve);

        // Barrel values
        m_barrel.insert("1_status", int(BarrelStatus::error));
        m_barrel.insert("1_name", "B1");
        m_barrel.insert("2_status", int(BarrelStatus::error));
        m_barrel.insert("2_name", "B2");
        m_barrel.insert("3_status", int(BarrelStatus::error));
        m_barrel.insert("3_name", "B3");
        m_barrel.insert("4_status", int(BarrelStatus::error));
        m_barrel.insert("4_name", "B4");
        m_barrel.insert("5_status", int(BarrelStatus::error));
        m_barrel.insert("5_name", "B5");
        m_barrel.insert("6_status", int(BarrelStatus::error));
        m_barrel.insert("6_name", "B6");
        emit_barrelChanged(m_barrel);

        // Pressure values
        m_pressure.insert("1_status", int(PressureStatus::error));
        m_pressure.insert("1_name", "P1");
        m_pressure.insert("1_value", "0");
        m_pressure.insert("2_status", int(PressureStatus::error));
        m_pressure.insert("2_name", "P2");
        m_pressure.insert("2_value", "0");
        m_pressure.insert("3_status", int(PressureStatus::error));
        m_pressure.insert("3_name", "P3");
        m_pressure.insert("3_value", "0");
        m_pressure.insert("4_status", int(PressureStatus::error));
        m_pressure.insert("4_name", "P4");
        m_pressure.insert("4_value", "0");
        m_pressure.insert("5_status", int(PressureStatus::error));
        m_pressure.insert("5_name", "P5");
        m_pressure.insert("5_value", "0");
        m_pressure.insert("6_status", int(PressureStatus::error));
        m_pressure.insert("6_name", "P6");
        m_pressure.insert("6_value", "0");
        m_pressure.insert("7_status", int(PressureStatus::error));
        m_pressure.insert("7_name", "P7");
        m_pressure.insert("7_value", "0");
        m_pressure.insert("8_status", int(PressureStatus::error));
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

        // When thread started rear LEDs
        connect(&hardware, &Hardware::Access::started, this, &SystemValues::hardwardThreadStart);

        // Connect object signals to hardware slots and visa versa
        connect(this, &SystemValues::hardwareRequest, &hardware, &Hardware::Access::hardwareAccess);

        // Valve status
        connect(&hardware, &Hardware::Access::emit_valveOpened, this, &SystemValues::valveChange);
        connect(&hardware, &Hardware::Access::emit_valveClosed, this, &SystemValues::valveChange);

        // Pump status
        connect(&hardware, &Hardware::Access::emit_pumpEnabled, this, &SystemValues::pumpChange);
        connect(&hardware, &Hardware::Access::emit_pumpDisabled, this, &SystemValues::pumpChange);

        // Guage status
        connect(&hardware, &Hardware::Access::emit_guageReadVacuum, this, &SystemValues::guageReadingChanged);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::MachineContainer::emit_vacuumMachineStopped, this, &SystemValues::startVacuumGuages);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::MachineContainer::emit_guageTripped, this, &SystemValues::guageTripChanged);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::MachineContainer::emit_vacuumTripMachineStopped, this, &SystemValues::startVacuumTripGuages);

        // Barrell status

    }

    QString SystemValues::currentMode()
    {
        QString enableKey = (m_control["manual_auto"] == false) ? "manual_control_enabled" : "auto_control_enabled" ;
        enableKey = (m_control["remote"] == true) ? "remote_control_enabled" : enableKey ;

        return enableKey;
    }

    void SystemValues::startVacuumGuages()
    {
        m_experimentEngine->machines().startReadingVacuumGuages(currentMode());
    }

    void SystemValues::stopVacuumGuages()
    {
         m_experimentEngine->machines().stopReadVacuum();
    }

    void SystemValues::startVacuumTripGuages()
    {
        m_experimentEngine->machines().startReadingTripVacuumGuages(currentMode());
    }

    void SystemValues::stopVacuumTripGuages()
    {
        m_experimentEngine->machines().stopReadTripVacuum();
    }

    void SystemValues::hardwardThreadStart()
    {
        // Update view froms settings
        refreshGeneralSettings();

        // Start guage state machine
        startVacuumGuages();
        startVacuumTripGuages();
    }

    void SystemValues::guageTripChanged(int group, bool state)
    {
        // Update valve
        m_pressure.insert(QString::number(group) + "_trip", state);

        // We'll let guageReadingChanged update the trip on LCD
    }

    void SystemValues::guageReadingChanged(QVariantMap data)
    {
        // Get info we require
        double presssure = data["pressure_mbar"].toDouble();
        double pressureBar = presssure / 1000;
        //qDebug() << presssure <<"vs"<<pressureBar;
        int guageId = data["guage_id"].toInt();
        QString guageIdS = data["guage_id"].toString();

        // Assum there's a guage error?
        int state = 3;

        // If not error are we within the pressure boundary for chamber?
        if(data["view_status"].toInt() != 3 && guageId < 7 && m_pressure[guageIdS+"_trip"].toBool() == false)
        {
            // Get chamber settings
            auto chamber = m_settings->general()->chamber(guageId);

            // Get pressure boundies
            double upper = chamber["upper_set_point"].toDouble();
            // int lower = chamber["lower_set_point"].toInt();

            // Are we within pressure?
            state = (presssure < upper) ? 1 : 2 ;
        }
        else if(data["view_status"].toInt() != 3 && guageId >= 7 && m_pressure[guageIdS+"_trip"].toBool() == false)
        {
            // Get pump settings
            auto pump = m_settings->general()->pump(guageId - 6);

            // Get pressure boundies
            double upper = pump["upper_set_point"].toDouble();
            // int lower = pump["lower_set_point"].toInt();

            // Are we within pressure?
            state = (presssure < upper) ? 1 : 2 ;
        }
        else if(m_pressure[guageIdS+"_trip"].toBool() == true) // The guage has tripped
        {
            state = 4;
        }

        // Update guage
        m_pressure.insert(guageIdS + "_status", state);

        // Update barrel
        if(guageId < 7)
        {
            m_barrel.insert(guageIdS + "_status", state);
            emit_barrelChanged(m_barrel);
        }

        // Save the pressure value (rounded)
        m_pressure.insert(guageIdS + "_value", QString::number(pressureBar, 'e', 2));

        // Let people know we've updated
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
        m_commentSettings.insert("1_value", generalChamber_1["comment"]);
        m_commentSettings.insert("2_value", generalChamber_2["comment"]);
        m_commentSettings.insert("3_value", generalChamber_3["comment"]);
        m_commentSettings.insert("4_value", generalChamber_4["comment"]);
        m_commentSettings.insert("5_value", generalChamber_5["comment"]);
        m_commentSettings.insert("6_value", generalChamber_6["comment"]);
        emit emit_commentSettingsChanged(m_commentSettings);
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

        m_barrelSettings.insert("manual_safety_valve", general->manualSafetyValve());
        m_barrelSettings.insert("ignore_trips", general->ignoreTrips());

        m_barrelSettings.insert("1_manual", generalChamber_1["manual_control_enabled"]);
        m_barrelSettings.insert("1_auto", generalChamber_1["auto_control_enabled"]);
        m_barrelSettings.insert("1_lower_set_point", generalChamber_1["lower_set_point"]);
        m_barrelSettings.insert("1_upper_set_point", generalChamber_1["upper_set_point"]);
        m_barrelSettings.insert("1_alarm_pressure", generalChamber_1["alarm_pressure"]);
        m_barrelSettings.insert("1_alarm_time", generalChamber_1["alarm_time"]);
        m_barrelSettings.insert("1_leak_detection", generalChamber_1["leak_detection"]);
        m_barrelSettings.insert("1_leak_period", generalChamber_1["leak_period"]);
        m_barrelSettings.insert("1_leak_max", generalChamber_1["leak_max"]);
        m_barrelSettings.insert("1_heavy_load", generalChamber_1["heavy_load"]);
        m_barrelSettings.insert("1_pumping_time", generalChamber_1["pumping_time"]);
        m_barrelSettings.insert("1_leak_delay", generalChamber_1["leak_delay"]);
        m_barrelSettings.insert("1_barrel_delay", generalChamber_1["barrel_delay"]);

        m_barrelSettings.insert("2_manual", generalChamber_2["manual_control_enabled"]);
        m_barrelSettings.insert("2_auto", generalChamber_2["auto_control_enabled"]);
        m_barrelSettings.insert("2_lower_set_point", generalChamber_2["lower_set_point"]);
        m_barrelSettings.insert("2_upper_set_point", generalChamber_2["upper_set_point"]);
        m_barrelSettings.insert("2_alarm_pressure", generalChamber_2["alarm_pressure"]);
        m_barrelSettings.insert("2_alarm_time", generalChamber_2["alarm_time"]);
        m_barrelSettings.insert("2_leak_detection", generalChamber_2["leak_detection"]);
        m_barrelSettings.insert("2_leak_period", generalChamber_2["leak_period"]);
        m_barrelSettings.insert("2_leak_max", generalChamber_2["leak_max"]);
        m_barrelSettings.insert("2_heavy_load", generalChamber_2["heavy_load"]);
        m_barrelSettings.insert("2_pumping_time", generalChamber_2["pumping_time"]);
        m_barrelSettings.insert("2_leak_delay", generalChamber_2["leak_delay"]);
        m_barrelSettings.insert("2_barrel_delay", generalChamber_2["barrel_delay"]);

        m_barrelSettings.insert("3_manual", generalChamber_3["manual_control_enabled"]);
        m_barrelSettings.insert("3_auto", generalChamber_3["auto_control_enabled"]);
        m_barrelSettings.insert("3_lower_set_point", generalChamber_3["lower_set_point"]);
        m_barrelSettings.insert("3_upper_set_point", generalChamber_3["upper_set_point"]);
        m_barrelSettings.insert("3_alarm_pressure", generalChamber_3["alarm_pressure"]);
        m_barrelSettings.insert("3_alarm_time", generalChamber_3["alarm_time"]);
        m_barrelSettings.insert("3_leak_detection", generalChamber_3["leak_detection"]);
        m_barrelSettings.insert("3_leak_period", generalChamber_3["leak_period"]);
        m_barrelSettings.insert("3_leak_max", generalChamber_3["leak_max"]);
        m_barrelSettings.insert("3_heavy_load", generalChamber_3["heavy_load"]);
        m_barrelSettings.insert("3_pumping_time", generalChamber_3["pumping_time"]);
        m_barrelSettings.insert("3_leak_delay", generalChamber_3["leak_delay"]);
        m_barrelSettings.insert("3_barrel_delay", generalChamber_3["barrel_delay"]);

        m_barrelSettings.insert("4_manual", generalChamber_4["manual_control_enabled"]);
        m_barrelSettings.insert("4_auto", generalChamber_4["auto_control_enabled"]);
        m_barrelSettings.insert("4_lower_set_point", generalChamber_4["lower_set_point"]);
        m_barrelSettings.insert("4_upper_set_point", generalChamber_4["upper_set_point"]);
        m_barrelSettings.insert("4_alarm_pressure", generalChamber_4["alarm_pressure"]);
        m_barrelSettings.insert("4_alarm_time", generalChamber_4["alarm_time"]);
        m_barrelSettings.insert("4_leak_detection", generalChamber_4["leak_detection"]);
        m_barrelSettings.insert("4_leak_period", generalChamber_4["leak_period"]);
        m_barrelSettings.insert("4_leak_max", generalChamber_4["leak_max"]);
        m_barrelSettings.insert("4_heavy_load", generalChamber_4["heavy_load"]);
        m_barrelSettings.insert("4_pumping_time", generalChamber_4["pumping_time"]);
        m_barrelSettings.insert("4_leak_delay", generalChamber_4["leak_delay"]);
        m_barrelSettings.insert("4_barrel_delay", generalChamber_4["barrel_delay"]);

        m_barrelSettings.insert("5_manual", generalChamber_5["manual_control_enabled"]);
        m_barrelSettings.insert("5_auto", generalChamber_5["auto_control_enabled"]);
        m_barrelSettings.insert("5_lower_set_point", generalChamber_5["lower_set_point"]);
        m_barrelSettings.insert("5_upper_set_point", generalChamber_5["upper_set_point"]);
        m_barrelSettings.insert("5_alarm_pressure", generalChamber_5["alarm_pressure"]);
        m_barrelSettings.insert("5_alarm_time", generalChamber_5["alarm_time"]);
        m_barrelSettings.insert("5_leak_detection", generalChamber_5["leak_detection"]);
        m_barrelSettings.insert("5_leak_period", generalChamber_5["leak_period"]);
        m_barrelSettings.insert("5_leak_max", generalChamber_5["leak_max"]);
        m_barrelSettings.insert("5_heavy_load", generalChamber_5["heavy_load"]);
        m_barrelSettings.insert("5_pumping_time", generalChamber_5["pumping_time"]);
        m_barrelSettings.insert("5_leak_delay", generalChamber_5["leak_delay"]);
        m_barrelSettings.insert("5_barrel_delay", generalChamber_5["barrel_delay"]);

        m_barrelSettings.insert("6_manual", generalChamber_6["manual_control_enabled"]);
        m_barrelSettings.insert("6_auto", generalChamber_6["auto_control_enabled"]);
        m_barrelSettings.insert("6_lower_set_point", generalChamber_6["lower_set_point"]);
        m_barrelSettings.insert("6_upper_set_point", generalChamber_6["upper_set_point"]);
        m_barrelSettings.insert("6_alarm_pressure", generalChamber_6["alarm_pressure"]);
        m_barrelSettings.insert("6_alarm_time", generalChamber_6["alarm_time"]);
        m_barrelSettings.insert("6_leak_detection", generalChamber_6["leak_detection"]);
        m_barrelSettings.insert("6_leak_period", generalChamber_6["leak_period"]);
        m_barrelSettings.insert("6_leak_max", generalChamber_6["leak_max"]);
        m_barrelSettings.insert("6_heavy_load", generalChamber_6["heavy_load"]);
        m_barrelSettings.insert("6_pumping_time", generalChamber_6["pumping_time"]);
        m_barrelSettings.insert("6_leak_delay", generalChamber_6["leak_delay"]);
        m_barrelSettings.insert("6_barrel_delay", generalChamber_6["barrel_delay"]);


        // Pump params
        auto generalPump_1 = general->pump(1);
        auto generalPump_2 = general->pump(2);

        m_pumpSettings.insert("default_pump", general->defaultPump());

        m_pumpSettings.insert("1_lower_set_point", generalPump_1["lower_set_point"]);
        m_pumpSettings.insert("1_upper_set_point", generalPump_1["upper_set_point"]);
        m_pumpSettings.insert("1_warm_up", generalPump_1["warm_up"]);
        m_pumpSettings.insert("1_alarm_pressure", generalPump_1["alarm_pressure"]);
        m_pumpSettings.insert("1_alarm_time", generalPump_1["alarm_time"]);
        m_pumpSettings.insert("1_auto", generalPump_1["auto_control_enabled"]);
        m_pumpSettings.insert("1_manual", generalPump_1["manual_control_enabled"]);
        m_pumpSettings.insert("1_pump_void", generalPump_1["pump_void"]);
        m_pumpSettings.insert("1_pump_manifold_void", generalPump_1["pump_manifold_void"]);
        m_pumpSettings.insert("1_leak_detection", generalPump_1["leak_detection"]);
        m_pumpSettings.insert("1_leak_period", generalPump_1["leak_period"]);
        m_pumpSettings.insert("1_leak_max", generalPump_1["leak_max"]);
        m_pumpSettings.insert("1_manifold_delay", generalPump_1["manifold_delay"]);

        m_pumpSettings.insert("2_lower_set_point", generalPump_2["lower_set_point"]);
        m_pumpSettings.insert("2_upper_set_point", generalPump_2["upper_set_point"]);
        m_pumpSettings.insert("2_warm_up", generalPump_2["warm_up"]);
        m_pumpSettings.insert("2_alarm_pressure", generalPump_2["alarm_pressure"]);
        m_pumpSettings.insert("2_alarm_time", generalPump_2["alarm_time"]);
        m_pumpSettings.insert("2_auto", generalPump_2["auto_control_enabled"]);
        m_pumpSettings.insert("2_manual", generalPump_2["manual_control_enabled"]);
        m_pumpSettings.insert("2_pump_void", generalPump_2["pump_void"]);
        m_pumpSettings.insert("2_pump_manifold_void", generalPump_2["pump_manifold_void"]);
        m_pumpSettings.insert("2_leak_detection", generalPump_2["leak_detection"]);
        m_pumpSettings.insert("2_leak_period", generalPump_2["leak_period"]);
        m_pumpSettings.insert("2_leak_max", generalPump_2["leak_max"]);
        m_pumpSettings.insert("2_manifold_delay", generalPump_2["manifold_delay"]);



        // Update the interface
        emit emit_barrelSettingsChanged(m_barrelSettings);
        emit emit_pumpSettingsChanged(m_pumpSettings);
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
        QString enableKey = currentMode();

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


    void SystemValues::autoControlFailed()
    {
        // Set control
        m_control["manual_auto"] = false;

        // Update view
        setGeneralSettingEnables();

        // Stop (with will then restart) vacuum gauge state machine
        stopVacuumGuages();
        stopVacuumTripGuages();

        // Tell everyone we've updated
        emit_controlChanged(m_control);
    }

    void SystemValues::autoControlFinished()
    {
        // Set control
        m_control["manual_auto"] = false;

        // Update view
        setGeneralSettingEnables();

        // Stop (with will then restart) vacuum gauge state machine
        stopVacuumGuages();
        stopVacuumTripGuages();

        // Tell everyone we've updated
        emit_controlChanged(m_control);
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

        // Stop (with will then restart) vacuum gauge state machine
        stopVacuumGuages();
        stopVacuumTripGuages();

        // Tell everyone we've updated
        emit_controlChanged(m_control);

        // If auto control
        if(type == "manual_auto" && state)
        {
            m_experimentEngine->machines().startAutoControl();
        }
        else if(type == "manual_auto" && !state)
        {
            if(m_experimentEngine->machines().isRunningAutoControl())
                m_experimentEngine->machines().stopAutoControl();
        }
    }

}}}

