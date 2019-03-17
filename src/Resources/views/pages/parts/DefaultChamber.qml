import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import "../../parts/rig"

Rectangle {
    id: rigunit
    width: rig.rowsize
    height: 67
    color: "transparent"

    // Set
    property int set: 1

    // Valve info
    property bool valve_enabled: false
    property int valve_status: 1
    property string valve_name: "V?"

    // Barrel info
    property bool barrel_enabled: false
    property int barrel_status: 1
    property string barrel_name: "B?"

    // Pressure sensor info
    property bool pressure_enabled: false
    property int pressure_status: 1
    property double pressure_value: 0.00
    property string pressure_name: "P?"

    // Comment box
    property string comment: "No comment exists"

    // Set number
    Rectangle {
        width: 16
        height: 16
        x: -17
        y: 0
        color: "transparent"
        border.color: "#bebebd"
        border.width: 2
        radius: 8
        Text{
            text: rigunit.set
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            height: parent.height
            width: parent.width
            font.pointSize: 7
            color: "#5b5b5b"
        }
        Rectangle
        {
            width: 1
            height: 245
            color: "#d5d5d4"
            anchors.top: parent.bottom
            //anchors.topMargin: -8
            anchors.left: parent.left
            anchors.leftMargin: 7

            Rectangle
            {
                width: 20
                height: 1
                color: "#d5d5d4"
                anchors.top: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -5
            }
        }
    }

    // Pipe from barrel to comment
    Rectangle {
        width: 10
        height: 80
        x: 86
        y: 45
        color: "#bebebd"
    }

    // Pipe from pressure sensor
    Rectangle {
        width: 20
        height: 10
        x: 75
        y: 80
        color: "#bebebd"
    }

    // Pipe from barrel to valve
    Rectangle {
        width: 10
        height: 79
        x: 63
        y: 191
        color: "#bebebd"
    }

    Column
    {
        spacing: 11

        Comment{
            width: rig.rowsize - 5
            info: rigunit.comment
            set: rigunit.set
        }
        Pressure{
            enabled: rigunit.pressure_enabled
            state: rigunit.pressure_status
            value: rigunit.pressure_value
            name: rigunit.pressure_name
            set: rigunit.set
        }
        Barrel{
            anchors.left: parent.left
            anchors.leftMargin: 43
            enabled: rigunit.barrel_enabled
            state: rigunit.barrel_status
            name: rigunit.barrel_name
            set: rigunit.set
        }
        Valve{
            anchors.left: parent.left
            anchors.leftMargin: 54
            enabled: rigunit.valve_enabled
            state: rigunit.valve_status
            name: rigunit.valve_name
            set: rigunit.set
       }
    }

}
