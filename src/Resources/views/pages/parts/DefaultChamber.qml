import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import "../../parts/rig"

Rectangle {
    id: rigunit
    width: rig.rowsize
    height: 67
    color: "transparent"

    // Valve info
    property int valve_status: 1
    property string valve_name: "V?"

    // Barrel info
    property int barrel_status: 1
    property string barrel_name: "B?"

    // Pressure sensor info
    property double pressure_status: 1
    property double pressure_value: 0.00
    property string pressure_name: "P?"

    // Comment box
    property string comment: "No comment exists"

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
        height: 75
        x: 63
        y: 195
        color: "#bebebd"
    }

    Column
    {
        spacing: 15

        Comment{
            width: rig.rowsize - 5
            info: rigunit.comment
        }
        Pressure{
            state: rigunit.pressure_status
            value: rigunit.pressure_value
            name: rigunit.pressure_name
        }
        Barrel{
            anchors.left: parent.left
            anchors.leftMargin: 43
            state: rigunit.barrel_status
            name: rigunit.barrel_name
        }
        Valve{
            anchors.left: parent.left
            anchors.leftMargin: 54
            state: rigunit.valve_status
            name: rigunit.valve_name
       }
    }

}
