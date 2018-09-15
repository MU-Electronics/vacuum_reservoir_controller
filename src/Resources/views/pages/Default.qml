import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import "parts"
import "../parts/rig"

Item{
    id:rig

    //width: parent.width
    //height: parent.height

    property double rowsize: 101

    Popup {
        id: popup
        width: 400
        height: 200
        x: parent.width / 4
        y: parent.height / 4
        closePolicy: Popup.NoAutoClose
        modal: true
        visible: SystemValuesManager.initialising;

        Text{
            id: pleasewait
            text: "System initialising, please wait..."
            anchors.left: parent.Left
            y: parent.height / 4
            width: parent.width
            height: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize : 15
        }

        BusyIndicator {
            running: true
            x: (parent.width / 2)-30

            anchors.top: pleasewait.bottom
            anchors.topMargin: 20
        }
    }

    Row
    {
        spacing: 30
        width: parent.width
        height: parent.height
        anchors.left: parent.left;
        anchors.leftMargin: 20

        anchors.top: parent.top;
        anchors.topMargin: 10


        DefaultChamber
        {
            set: 1

            valve_status: SystemValuesManager.valveState["1_status"];
            valve_name: SystemValuesManager.valveState["1_name"];

            barrel_status: SystemValuesManager.barrelState["1_status"];
            barrel_name: SystemValuesManager.barrelState["1_name"];

            pressure_status: SystemValuesManager.pressureState["1_status"];
            pressure_value: SystemValuesManager.pressureState["1_value"];
            pressure_name: SystemValuesManager.pressureState["1_name"];

            comment: SystemValuesManager.commentState["1_value"];
        }

        DefaultChamber
        {
            set: 2

            valve_status: SystemValuesManager.valveState["2_status"];
            valve_name: SystemValuesManager.valveState["2_name"];

            barrel_status: SystemValuesManager.barrelState["2_status"];
            barrel_name: SystemValuesManager.barrelState["2_name"];

            pressure_status: SystemValuesManager.pressureState["2_status"];
            pressure_value: SystemValuesManager.pressureState["2_value"];
            pressure_name: SystemValuesManager.pressureState["2_name"];

            comment: SystemValuesManager.commentState["2_value"];
        }

        DefaultChamber
        {
            set: 3

            valve_status: SystemValuesManager.valveState["3_status"];
            valve_name: SystemValuesManager.valveState["3_name"];

            barrel_status: SystemValuesManager.barrelState["3_status"];
            barrel_name: SystemValuesManager.barrelState["3_name"];

            pressure_status: SystemValuesManager.pressureState["3_status"];
            pressure_value: SystemValuesManager.pressureState["3_value"];
            pressure_name: SystemValuesManager.pressureState["3_name"];

            comment: SystemValuesManager.commentState["3_value"];
        }

        DefaultChamber
        {
            set: 4

            valve_status: SystemValuesManager.valveState["4_status"];
            valve_name: SystemValuesManager.valveState["4_name"];

            barrel_status: SystemValuesManager.barrelState["4_status"];
            barrel_name: SystemValuesManager.barrelState["4_name"];

            pressure_status: SystemValuesManager.pressureState["4_status"];
            pressure_value: SystemValuesManager.pressureState["4_value"];
            pressure_name: SystemValuesManager.pressureState["4_name"];

            comment: SystemValuesManager.commentState["4_value"];
        }

        DefaultChamber
        {
            set: 5

            valve_status: SystemValuesManager.valveState["5_status"];
            valve_name: SystemValuesManager.valveState["5_name"];

            barrel_status: SystemValuesManager.barrelState["5_status"];
            barrel_name: SystemValuesManager.barrelState["5_name"];

            pressure_status: SystemValuesManager.pressureState["5_status"];
            pressure_value: SystemValuesManager.pressureState["5_value"];
            pressure_name: SystemValuesManager.pressureState["5_name"];

            comment: SystemValuesManager.commentState["5_value"];
        }

        DefaultChamber
        {
            set: 6

            valve_status: SystemValuesManager.valveState["6_status"];
            valve_name: SystemValuesManager.valveState["6_name"];

            barrel_status: SystemValuesManager.barrelState["6_status"];
            barrel_name: SystemValuesManager.barrelState["6_name"];

            pressure_status: SystemValuesManager.pressureState["6_status"];
            pressure_value: SystemValuesManager.pressureState["6_value"];
            pressure_name: SystemValuesManager.pressureState["6_name"];

            comment: SystemValuesManager.commentState["6_value"];
        }
    }


    // Valve joining pipe
    Rectangle {
        width: 665
        height: 10
        x: 83
        y: 280
        color: "#bebebd"
    }

    // Valve joing pipe drop down pipe
    Rectangle {
        width: 10
        height: 75
        x: 400
        y: 280
        color: "#bebebd"
    }

    // Pipe to join drop down pipe to pumps
    Rectangle {
        width: 600
        height: 10
        x: 100
        y: 353
        color: "#bebebd"
    }


    // Set number 7
    Rectangle {
        width: 16
        height: 16
        x: 70
        y: 293
        color: "transparent"
        border.color: "#bebebd"
        border.width: 2
        radius: 8
        Text{
            text: "7"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            height: parent.height
            width: parent.width
            font.pointSize: 7
            color: "#5b5b5b"
        }
        Rectangle
        {
            width: 300
            height: 1
            color: "#d5d5d4"
            anchors.top: parent.top
            anchors.topMargin: 7
            anchors.left: parent.right
            anchors.leftMargin: 0

            Rectangle
            {
                width: 1
                height: 20
                color: "#d5d5d4"
                anchors.top: parent.bottom
                anchors.left: parent.right
                anchors.topMargin: -5
            }
        }
    }

    // Pump 1 valve
    Valve{
        x: 340
        y: 332
        settingx: -312
        settingy: 310
        waitx: -10
        waity: -220
        set: 7
        state: SystemValuesManager.valveState["7_status"];
        name: SystemValuesManager.valveState["7_name"];
        angle: false
    }

    // Pump one pressure sensor
    Pressure{
        x:205
        y:305
        settingx: -165
        settingy: -275
        waitx: 130
        waity: -185
        set: 7
        state: SystemValuesManager.pressureState["7_status"];
        name: SystemValuesManager.pressureState["7_name"];
        value: SystemValuesManager.pressureState["7_value"];

        Rectangle {
            width: 10
            height: 15
            anchors.top: parent.bottom;
            anchors.left: parent.left
            anchors.leftMargin: 32
            color: "#bebebd"
        }
    }

    Pump{
        x:82
        y:320
        waitx: 255
        waity: -200
        set: 7
        state: SystemValuesManager.pumpState["1_status"];
        name: SystemValuesManager.pumpState["1_name"];
    }




    // Set number 8
    Rectangle {
        width: 16
        height: 16
        x: 744
        y: 293
        color: "transparent"
        border.color: "#bebebd"
        border.width: 2
        radius: 8
        Text{
            text: "8"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            height: parent.height
            width: parent.width
            font.pointSize: 7
            color: "#5b5b5b"
        }
        Rectangle
        {
            width: 321
            height: 1
            color: "#d5d5d4"
            anchors.top: parent.top
            anchors.topMargin: 7
            anchors.right: parent.left
            anchors.leftMargin: 0

            Rectangle
            {
                width: 1
                height: 20
                color: "#d5d5d4"
                anchors.top: parent.bottom
                anchors.left: parent.left
                anchors.topMargin: -5
            }
        }
    }

    // Pump 2 valve
    Valve{
        x: 490
        y: 332
        settingx: -312
        settingy: 460
        waitx: -160
        waity: -220
        set: 8
        state: SystemValuesManager.valveState["8_status"];
        name: SystemValuesManager.valveState["8_name"];
        angle: false
    }

    // Pump two pressure sensor
    Pressure{
        x:550
        y:305
        settingx: -510
        settingy: -275
        waitx: -215
        waity: -185
        set: 8
        state: SystemValuesManager.pressureState["8_status"];
        name: SystemValuesManager.pressureState["8_name"];
        value: SystemValuesManager.pressureState["8_value"];

        Rectangle {
            width: 10
            height: 15
            anchors.top: parent.bottom;
            anchors.left: parent.left
            anchors.leftMargin: 32
            color: "#bebebd"
        }
    }

    Pump{
        x:723
        y:320
        set: 8
        settingx: -683
        settingy: -290
        waitx: -380
        waity: -200
        state: SystemValuesManager.pumpState["2_status"];
        name: SystemValuesManager.pumpState["2_name"];
        flip: false
    }


}
