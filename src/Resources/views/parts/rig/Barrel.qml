import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "settings"
import "../"

Rectangle
{
    id: barrel

    // Chamber set
    property int set: 1

    // Is enabled
    property bool enabled: false

    // State of barrel
    property int state: 1

    // Barrel name
    property string name: "B1"

    // Settings location
    property int settingx: 0
    property int settingy: 0

    // Colours depend on barrel state
    property string colour1: {
        if(barrel.enabled)
        {
            if(barrel.state === 1) // Within pressure tolerance
                return "#06d652" // Green

            if(barrel.state === 2) // Not within perssure tolerance
                return "#c78f28" // Orange

            if(barrel.state === 3) // Error
                return "#cb1c29" // Red
        }

        // Disabled id: 4
        return "#cac8c8"
    }

    // Colours depend on barrel state
    property string colour2: {
        if(barrel.enabled)
        {
            if(barrel.state === 1) // Within pressure tolerance
                return "#0caf48" // Green

            if(barrel.state === 2) // Not within perssure tolerance
                return "#ad7b20" // Orange

            if(barrel.state === 3) // Error
                return "#a81520" // Red
        }

        // Disabled id: 4
        return "#dbdbdb"
    }

    // Colours depend on barrel state
    property string tcolour: {
        if(barrel.enabled)
        {
            if(barrel.state === 1) // Within pressure tolerance
                return "#ffffff"

            if(barrel.state === 2) // Sensor Range
                return "#ffffff"

            if(barrel.state === 3) // Sensor Error
                return "#ffffff"
        }

        // Disabled id: 4
        return "#6d6d6d"
    }

    height: 80
    width: 100
    color: "transparent"

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if(SystemValuesManager.showSettings())
            {
                console.log("showing");
                settingsloader.source = "../../parts/rig/settings/BarrelSettings.qml"
                settingsloader.active = true
                settingsloader.focus = true
                settingsloader.visible = true
            }
        }
    }

    BusyPopup{
        status: settingsloader.status
        xpos: {
            if(barrel.set == 1)
            {
                return 280;
            }

            return (280 - (131 * (barrel.set - 1)))
        }
        ypos: -10
    }

    Loader {
        id: settingsloader
        source: ""
        active: false
        asynchronous: true
        visible: false
        focus: false
        onLoaded: {
            item.set = barrel.set
            item.settingsLoaderAliase = settingsloader
            if(barrel.settingx != 0)
                item.settingx = barrel.settingx
            if(barrel.settingy != 0)
                item.settingy = barrel.settingy
            item.open();
        }
    }

    // Top ring
    Rectangle
    {
        id: topring
        width: 60
        height: 10
        color: "#bebebd"
        radius: 5
        anchors.left: parent.left
        anchors.top: parent.top

        Rectangle
        {
            width: 30
            height: 10
            color: "#a8a8a7"
            radius: 5
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: parent.top
        }

        Rectangle
        {
            width: 12
            height: 10
            color: "#a8a8a7"
            anchors.left: parent.left
            anchors.leftMargin: 13
            anchors.top: parent.top
        }
    }

    // Top section
    Rectangle
    {
        id: topsection
        width: 60
        height: 25
        color: barrel.colour1
        anchors.left: topring.left
        anchors.leftMargin: -5
        anchors.top: topring.bottom

        Rectangle
        {
            width: 30
            height: 25
            color: barrel.colour2
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.top: parent.top
        }

        Text{
            width: 60
            height: 25
            text: barrel.name
            font.weight: Font.DemiBold
            font.pointSize: 9
            color: barrel.tcolour;
            anchors.top: parent.top;
            anchors.topMargin: 5
            anchors.left: parent.left;
            horizontalAlignment: Text.AlignHCenter
        }
    }

    // Mid ring
    Rectangle
    {
        id: midring
        width: 60
        height: 10
        color: "#bebebd"
        radius: 5
        anchors.left: parent.left
        anchors.top: topsection.bottom

        Rectangle
        {
            width: 30
            height: 10
            color: "#a8a8a7"
            radius: 5
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: parent.top
        }

        Rectangle
        {
            width: 12
            height: 10
            color: "#a8a8a7"
            anchors.left: parent.left
            anchors.leftMargin: 13
            anchors.top: parent.top
        }
    }

    // Bottom section
    Rectangle
    {
        id: bottomsection
        width: 60
        height: 25
        color: barrel.colour1
        anchors.left: midring.left
        anchors.leftMargin: -5
        anchors.top: midring.bottom

        Rectangle
        {
            width: 30
            height: 25
            color: barrel.colour2
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.top: parent.top
        }

        Text{
            width: 60
            height: 25
            text: {
                if(barrel.enabled)
                {
                    if(barrel.state === 1)
                        return "Within"

                    if(barrel.state === 2)
                        return "Attention"

                    if(barrel.state === 3)
                        return "ERORR"
                }

                return "Disabled"
            }
            font.pointSize: 8
            color: barrel.tcolour;
            anchors.top: parent.top;
            anchors.topMargin: 5
            anchors.left: parent.left;
            horizontalAlignment: Text.AlignHCenter
        }


    }

    // Bottom ring
    Rectangle
    {
        width: 60
        height: 10
        color: "#bebebd"
        radius: 5
        anchors.left: parent.left
        anchors.top: bottomsection.bottom

        Rectangle
        {
            width: 30
            height: 10
            color: "#a8a8a7"
            radius: 5
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: parent.top
        }

        Rectangle
        {
            width: 12
            height: 10
            color: "#a8a8a7"
            anchors.left: parent.left
            anchors.leftMargin: 13
            anchors.top: parent.top
        }
    }

}
