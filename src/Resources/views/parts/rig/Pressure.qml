import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "settings"
import "../"

Rectangle
{
    id: pressure

    width: 75
    height: 36
    radius: 10

    // Chamber set
    property int set: 1

    // Is enabled
    property bool enabled: false

    // State of sensor
    property int state: 1

    // State of sensor
    property double value: 00.00

    // Sensor name
    property string name: "P?"

    // Colours depend on sensor state
    property string colour1: {
        if(pressure.enabled)
        {
            if(pressure.state === 1) // Sensor ok
                return "#dacf07"

            if(pressure.state === 2) // Sensor range
                return "#cb1c29"

            if(pressure.state === 3) // Sensor Error
                return "#c7a028"
        }

        // Disabled id: 4
        return "#c5c4c4"
    }

    // Colours depend on sensor state
    property string colour2: {
        if(pressure.enabled)
        {
            if(pressure.state === 1) // Sensor ok
                return "#c9be06"

            if(pressure.state === 2) // Sensor Range
                return "#bb1a26"

            if(pressure.state === 3) // Sensor Error
                return "#edc64e"
        }

        // Disabled id: 4
        return "#bebdbd"
    }

    // Colours depend on pump state
    property string tcolour: {
        if(pressure.enabled)
        {
            if(pressure.state === 1) // Sensor ok
                return "#ffffff"

            if(pressure.state === 2) // Sensor Range
                return "#ffffff"

            if(pressure.state === 3) // Sensor Error
                return "#ffffff"
        }

        // Disabled id: 4
        return "#ffffff"
    }

    // Settings location
    property int settingx: 0
    property int settingy: 0
    property int waitx: 0
    property int waity: 0

    color: pressure.colour1;

    border.width: 2
    border.color: pressure.colour2

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if(SystemValuesManager.showSettings())
            {
                settingsloader.source = "../../parts/rig/settings/PressureSettings.qml"
                settingsloader.active = true
                settingsloader.focus = true
                settingsloader.visible = true
            }
        }
    }

    BusyPopup{
        status: settingsloader.status
        xpos: {
            if(pressure.waitx != 0)
                return pressure.waitx

            if(pressure.set == 1)
            {
                return 320;
            }

            return (320 - (131 * (pressure.set - 1)))
        }
        ypos: {
            if(pressure.waity != 0)
                return pressure.waity
            return 35;
        }
    }

    Loader {
        id: settingsloader
        source: ""
        active: false
        asynchronous: true
        visible: false
        focus: false
        onLoaded: {
            item.set = pressure.set
            item.settingsLoaderAliase = settingsloader
            if(pressure.settingx != 0)
                item.settingx = pressure.settingx
            if(pressure.settingy != 0)
                item.settingy = pressure.settingy
            item.open();
        }
    }

    Text{
        id: pressurename
        text: pressure.name
        width: parent.width
        height: 15
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 9
        color: pressure.tcolour
        anchors.left: parent.left;
        anchors.top: parent.top
        anchors.topMargin: 3
        font.weight: Font.DemiBold
    }

    Text{
        text: (pressure.enabled === true) ? pressure.value + " mbar" : "disabled";
        width: parent.width
        height: 15
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 8
        color: pressure.tcolour
        anchors.left: parent.left;
        anchors.top: pressurename.top
        anchors.topMargin: 14
    }
}
