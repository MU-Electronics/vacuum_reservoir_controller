import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "settings"
import "../"

Rectangle
{
    id: pump

    // Chamber set
    property int set: 1

    // State of pump
    property int state: 1

    // Pump name
    property string name: "Pump ?"

    // Angle
    property bool flip: true

    // Settings location
    property int settingx: 0
    property int settingy: 0
    property int waitx: 0
    property int waity: 0

    // Colours depend on pump state
    property string colour1: {
        if(pump.state === 1) // Pump on
            return "#cb1c29"

        if(pump.state === 2) // Pump off
            return "#55965e"

        if(pump.state === 3) // Pump failed
            return "#c7a028"

        // Disabled id: 4
        return "#cac8c8"
    }

    // Colours depend on pump state
    property string colour2: {
        if(pump.state === 1) // Pump on
            return "#f12131"

        if(pump.state === 2) // Pump off
            return "#6bc077"

        if(pump.state === 3) // Pump failed
            return "#edc64e"

        // Disabled id: 4
        return "#dbdbdb"
    }

    // Colours depend on pump state
    property string tcolour: {
        if(pump.state === 1) // Pump on
            return "#ffffff"

        if(pump.state === 2) // Pump off
            return "#ffffff"

        if(pump.state === 3) // Pump failed
            return "#ffffff"

        // Disabled id: 4
        return "#6d6d6d"
    }

    // Colours depend on pump state
    property string tcolour2: {
        if(pump.state === 1) // Pump on
            return "#cb1c29"

        if(pump.state === 2) // Pump off
            return "#55965e"

        if(pump.state === 3) // Pump failed
            return "#c7a028"

        // Disabled id: 4
        return "#797979"
    }

    height: 70
    width: 100
    color: "transparent"



    BusyPopup{
        status: settingsloader.status
        xpos: {
            if(pump.waitx != 0)
                return pump.waitx

            if(pump.set == 7)
            {
                return 320;
            }

            return (320 - (600))
        }
        ypos: {
            if(pump.waity != 0)
                return pump.waity

            return 100;
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
            item.set = pump.set
            if(pump.settingx != 0)
                item.settingx = pump.settingx
            if(pump.settingy != 0)
                item.settingy = pump.settingy
            item.open();
        }
    }

    Rectangle
    {
        id: pumpmotor
        color: pump.colour1
        height: 22
        width: 25

        Text{
            text: {
                if(pump.state === 1)
                    return "On"

                if(pump.state === 2)
                    return "Off"

                if(pump.state === 3)
                    return "ERROR"

                return "Disabled"
            }
            width: 60
            height: 20
            horizontalAlignment: (pump.flip) ? Text.AlignLeft : Text.AlignRight ;
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 8
            color: pump.tcolour2

            // anchors.left: parent.right;
            anchors.left: (pump.flip) ? parent.right : undefined ;
            anchors.leftMargin: 10
            anchors.right: (!pump.flip) ? parent.left : undefined ;
            anchors.rightMargin: 10
        }

        Rectangle
        {
            color: pump.colour2
            height: 2
            width: 25
            anchors.top: parent.top
            anchors.topMargin: 2
        }

        Rectangle
        {
            color: pump.colour2
            height: 2
            width: 25
            anchors.top: parent.top
            anchors.topMargin: 6
        }

        Rectangle
        {
            color: pump.colour2
            height: 2
            width: 25
            anchors.top: parent.top
            anchors.topMargin: 10
        }

        Rectangle
        {
            color: pump.colour2
            height: 2
            width: 25
            anchors.top: parent.top
            anchors.topMargin: 14
        }

        Rectangle
        {
            color: pump.colour2
            height: 2
            width: 25
            anchors.top: parent.top
            anchors.topMargin: 18
        }

    }

    Rectangle
    {
        id: pumpbody
        color: pump.colour1
        height: 30
        width: 100
        anchors.top: pumpmotor.bottom

        anchors.left: (pump.flip) ? pumpmotor.left : undefined ;
        anchors.right: (!pump.flip) ? pumpmotor.right : undefined ;

        MouseArea {
            anchors.fill: parent
            onClicked: {
                settingsloader.source = "../../parts/rig/settings/PumpSettings.qml"
                settingsloader.active = true
                settingsloader.focus = true
                settingsloader.visible = true
            }
        }

        Text{
            text: pump.name
            font.weight: Font.DemiBold
            width: parent.width
            height: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 10
            color: pump.tcolour
            anchors.left: parent.left;
        }
    }

    Rectangle
    {
        width: 4
        height: 3
        color: "#a8a8a7"
        anchors.left: pumpbody.left
        anchors.leftMargin: 8
        anchors.top: pumpbody.bottom

        Rectangle
        {
            width: 20
            height: 3
            color: "#a8a8a7"
            anchors.left: parent.left
            anchors.leftMargin: -8
            anchors.top: parent.bottom
        }
    }

    Rectangle
    {
        width: 4
        height: 3
        color: "#a8a8a7"
        anchors.right: pumpbody.right
        anchors.rightMargin: 8
        anchors.top: pumpbody.bottom

        Rectangle
        {
            width: 20
            height: 3
            color: "#a8a8a7"
            anchors.right: parent.left
            anchors.rightMargin: -12
            anchors.top: parent.bottom
        }
    }


}
