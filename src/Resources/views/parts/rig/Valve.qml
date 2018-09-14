import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "settings"
import "../"

Rectangle
{
    id: valve

    // Chamber set
    property int set: 1

    // State of valve
    property int state: 2

    // Valve name
    property string name: "V?"

    // Angle
    property bool angle: true

    // Settings location
    property int settingx: 0
    property int settingy: 0
    property int waitx: 0
    property int waity: 0

    // Colours depend on valve state
    property string colour1: {
        if(valve.state === 1) // Open
            return "#cb1c29"

        if(valve.state === 2) // Closed
            return "#55965e"

        if(valve.state === 3) // Error
            return "#c7a028"

        // Disabled id: 4
        return "#cac8c8"
    }

    // Colours depend on valve state
    property string colour2: {
        if(valve.state === 1) // Open
            return "#f12131"

        if(valve.state === 2) // Closed
            return "#6bc077"

        if(valve.state === 3) // Error
            return "#edc64e"

        // Disabled id: 4
        return "#b0afaf"
    }

    // Colours depend on valve state
    property string tcolour: {
        if(valve.state === 1) // Open
            return "#cb1c29"

        if(valve.state === 2) // Closed
            return "#55965e"

        if(valve.state === 3) // Error
            return "#c7a028"

        // Disabled id: 4
        return "#797979"
    }

    height: 28
    width: 31
    color: "transparent"

    transform: Rotation {
        origin.x: 0;
        origin.y: 11;
        angle: (valve.angle) ? 0 : 90;
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            settingsloader.source = "../../parts/rig/settings/ValveSettings.qml"
            settingsloader.active = true
            settingsloader.focus = true
            settingsloader.visible = true
        }
    }

    BusyPopup{
        status: settingsloader.status
        xpos: {
            if(valve.waitx != 0)
                return valve.waitx

            if(valve.set == 1)
            {
                return 270;
            }

            return (270 - (131 * (valve.set - 1)))
        }
        ypos: {
            if(valve.waity != 0)
                return valve.waity

            return -110
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
            item.set = valve.set
            if(valve.settingx != 0)
                item.settingx = valve.settingx
            if(valve.settingy != 0)
                item.settingy = valve.settingy
            item.open();
        }
    }


    // Top Flang
    Rectangle
    {
        color: "#a8a8a7"
        height: 7
        width: 28

        Rectangle{
            color: "#b3b3b2"
            height: 3
            width: 3
            anchors.left: parent.left;
            anchors.leftMargin: 3
            anchors.top: parent.top;
            anchors.topMargin: 2;
        }

        Rectangle{
            color: "#b3b3b2"
            height: 3
            width: 3
            anchors.left: parent.left;
            anchors.leftMargin: 9
            anchors.top: parent.top;
            anchors.topMargin: 2;
        }

        Rectangle{
            color: "#b3b3b2"
            height: 3
            width: 3
            anchors.left: parent.left;
            anchors.leftMargin: 16
            anchors.top: parent.top;
            anchors.topMargin: 2;
        }

        Rectangle{
            color: "#b3b3b2"
            height: 3
            width: 3
            anchors.left: parent.left;
            anchors.leftMargin: 23
            anchors.top: parent.top;
            anchors.topMargin: 2;
        }
    }

    //Body
    Rectangle
    {
        id: valvebody
        color: valve.colour1
        height: 24
        width: 16
        anchors.left: parent.left;
        anchors.leftMargin: 6
        anchors.top: parent.top;
        anchors.topMargin: 7;

        Rectangle{
            color: (valve.state === 1) ? valve.colour2 : "transparent";
            height: 24
            width: 2
            anchors.left: parent.left;
            anchors.leftMargin: 7
            anchors.top: parent.top;
        }

        Rectangle{
            color: (valve.state === 2) ? valve.colour2 : "transparent";
            height: 2
            width: 16
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.topMargin: 10

        }

    }

    // Side body 1
    Rectangle
    {
        id: valvebody2
        color: "#a8a8a7"
        height: 16
        width: 3
        anchors.right: valvebody.left;
        anchors.top: valvebody.top;
        anchors.topMargin: 4;
    }

    // Side body 2
    Rectangle
    {
        id: valvebody3
        color: "#a8a8a7"
        height: 6
        width: 4
        anchors.right: valvebody2.left;
        anchors.top: valvebody2.top;
        anchors.topMargin: 5;
    }

    // Valve handle
    Rectangle
    {
        id: valvehandle
        color: valve.colour1
        height: 14
        width: 14
        anchors.right: valvebody3.left;
        anchors.top: valvebody3.top;
        anchors.topMargin: -4;

        Rectangle{
            color: valve.colour2
            height: 2
            width: 14
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.topMargin: 2
        }
        Rectangle{
            color: valve.colour2
            height: 2
            width: 14
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.topMargin: 6
        }
        Rectangle{
            color: valve.colour2
            height: 2
            width: 14
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.topMargin: 10
        }

        Text{
            width: 45
            horizontalAlignment: Text.AlignHCenter
            text: valve.name
            font.weight: Font.DemiBold
            font.pointSize: 9
            color: "#5b5b5b"
            anchors.right: parent.left;
            anchors.rightMargin: (valve.angle) ? 0 : -30;
            anchors.top: parent.top;
            anchors.topMargin: (valve.angle) ? -11 : -5;

            transform: Rotation {
                origin.x: 0;
                origin.y: 0;
                angle: (valve.angle) ? 0 : 270;
            }

            Text{
                width: 40
                text: {
                    if(valve.state === 1)
                        return "opened"

                    if(valve.state === 2)
                        return "closed"

                    if(valve.state === 3)
                        return "ERROR"

                    return "disabled"
                }
                font.pointSize: 8
                color: valve.tcolour;
                anchors.top: parent.top;
                anchors.topMargin: 13
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    // Bottom flang
    Rectangle
    {
        anchors.right: valvebody.right;
        anchors.rightMargin: -6
        anchors.top: valvebody.top;
        anchors.topMargin: 24;
        color: "#a8a8a7"
        height: 7
        width: 28

        Rectangle{
            color: "#b3b3b2"
            height: 3
            width: 3
            anchors.left: parent.left;
            anchors.leftMargin: 3
            anchors.top: parent.top;
            anchors.topMargin: 2;
        }

        Rectangle{
            color: "#b3b3b2"
            height: 3
            width: 3
            anchors.left: parent.left;
            anchors.leftMargin: 9
            anchors.top: parent.top;
            anchors.topMargin: 2;
        }

        Rectangle{
            color: "#b3b3b2"
            height: 3
            width: 3
            anchors.left: parent.left;
            anchors.leftMargin: 16
            anchors.top: parent.top;
            anchors.topMargin: 2;
        }

        Rectangle{
            color: "#b3b3b2"
            height: 3
            width: 3
            anchors.left: parent.left;
            anchors.leftMargin: 23
            anchors.top: parent.top;
            anchors.topMargin: 2;
        }
    }

}
