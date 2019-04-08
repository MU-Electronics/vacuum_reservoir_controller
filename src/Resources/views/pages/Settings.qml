import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "parts"

Item {
    id: settings

    property int rowPadding: 40

    anchors.top: parent.top
    anchors.topMargin: -10

    // Manual control safety valve or direct
    Rectangle{
        id: manualVsStateValveControl
        width: parent.width
        height: manualVsStateValveControlLabel.height + settings.rowPadding
        color: "#f2f2f2"
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 1

        Row
        {
            width: parent.width
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            Label{
                id: manualVsStateValveControlLabel
                text: "In manual control mode should only one barrel can be opened at any particular instance?"
                width: parent.width - 200
                Material.accent: Material.foreground
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                anchors.top: parent.top
                anchors.topMargin: 15
                font.pointSize: 11
                wrapMode: Text.WordWrap
            }

            CheckBox{
                id: barrelStateMachine
                width: 200
                checked: SystemValuesManager.barrelSettings["manual_safety_valve"]
                text: (barrelStateMachine.checked) ? "Single" : "Multi"

                onClicked: {
                    SettingsUpdaterManager.updateGeneralSettings(barrelStateMachine.checked, defaultPumpId.checked, ignoreTrips.checked)
                }
            }
        }
    }



    // Default pump
    Rectangle{
        id: defaultPump
        width: parent.width
        height: defaultPumpLabel.height + settings.rowPadding
        color: "#f9f9f9"
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: manualVsStateValveControl.bottom
        anchors.topMargin: 1

        Row
        {
            width: parent.width
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            Label{
                id: defaultPumpLabel
                text: "Which pump should be used as the default pump?"
                width: parent.width - 200
                Material.accent: Material.foreground
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                anchors.top: parent.top
                anchors.topMargin: 15
                font.pointSize: 11
                wrapMode: Text.WordWrap
            }

            CheckBox{
                id: defaultPumpId
                width: 200
                checked: (SystemValuesManager.pumpSettings["default_pump"] === 2) ? false : true
                text: (defaultPumpId.checked) ? "Pump 1" : "Pump 2"

                onClicked: {
                    SettingsUpdaterManager.updateGeneralSettings(barrelStateMachine.checked, defaultPumpId.checked, ignoreTrips.checked)
                }
            }
        }
    }



    // Default pump
    Rectangle{
        id: ignoreTripsContainer
        width: parent.width
        height: ignoreTripsContainerLabel.height + settings.rowPadding
        color: "#f2f2f2"
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: defaultPump.bottom
        anchors.topMargin: 1

        Row
        {
            width: parent.width
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            Label{
                id: ignoreTripsContainerLabel
                text: "Should ignore initial guage trips when starting automatic control?"
                width: parent.width - 200
                Material.accent: Material.foreground
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                anchors.top: parent.top
                anchors.topMargin: 15
                font.pointSize: 11
                wrapMode: Text.WordWrap
            }

            CheckBox{
                id: ignoreTrips
                width: 200
                checked: SystemValuesManager.barrelSettings["ignore_trips"]
                text: (ignoreTrips.checked) ? "Ignore" : "Account"

                onClicked: {
                    SettingsUpdaterManager.updateGeneralSettings(barrelStateMachine.checked, defaultPumpId.checked, ignoreTrips.checked)
                }
            }
        }
    }


}
