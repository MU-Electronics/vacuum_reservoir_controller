import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.VirtualKeyboard 2.2
import QtQuick.VirtualKeyboard.Settings 2.2
import QtQuick.Controls.Material 2.2 as Materials


Popup {

    id: popup

    property int set: 1

    property int pumpId: popup.set - 6

    // Settings location
    property int settingx: 0
    property int settingy: 0

    property var settingsLoaderAliase: 0

    width: 720
    height: 320
    //contentWidth: view.implicitWidth
    //contentHeight: view.implicitHeight
    x: {
        if(popup.settingx != 0)
            return popup.settingx

        if(popup.set == 7)
        {
            return -42;
        }

        return (-685)
    }

    y: {
        if(popup.settingy != 0)
            return popup.settingy

        return -290
    }
    closePolicy: Popup.NoAutoClose
    modal: true
    visible: false;


    contentItem: Item{
        id:item
        Text{
            id: title
            text: "Pump settings (set " + popup.set + ")"
            anchors.left: parent.Left
            anchors.top: parent.top
            anchors.topMargin: 0
            width: parent.width
            height: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize : 15
        }

        // Enableds + pump
        Rectangle{
            id: enables
            width: parent.width + 20
            height: 70
            color: "#f9f9f9"
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: title.bottom
            anchors.topMargin: 10

            Row
            {
                width: parent.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2

                Label{
                    text: "Automatic control"
                    width: 150
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                ComboBox {
                    id: pumpAutoState
                    width:150
                    model: ["Disable", "Enable"]
                    currentIndex: (SystemValuesManager.pumpState[popup.pumpId + "_auto"]) ? 1 : 0
                }

                Item{
                    width: 50
                    height: 50
                    Rectangle{
                        width:2
                        height: 50
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#f1f1f1"
                    }
                }

                Label{
                    text: "Manual control"
                    width: 150
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                ComboBox {
                    id: pumpManualState
                    width:150
                    model: ["Disable", "Enable"]
                    currentIndex: (SystemValuesManager.pumpState[popup.pumpId + "_manual"]) ? 1 : 0
                }
            }
        }


        // Sensors and valves
        Rectangle{
            id: sensors
            width: parent.width + 20
            height: 70
            color: "#f2f2f2"
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: enables.bottom
            anchors.topMargin: 1

            Row
            {
                width: parent.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2


                Label{
                    text: "Alarm (mbar)"
                    width: 165
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox {
                    id: pumpAlarmState
                    width: 150
                    value: SystemValuesManager.pumpState[popup.pumpId + "_alarm_pressure"]
                    from:1
                    to:500
                }

                Item{
                    width: 20
                    height: 50
                    Rectangle{
                        width:2
                        height: 50
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#f9f9f9"
                    }
                }

                Label{
                    text: "Alarm time (sec)"
                    width: 160
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox {
                    id: pumpAlarmTimeState
                    width: 200
                    value: SystemValuesManager.pumpState[popup.pumpId + "_alarm_time"]
                    from:1
                    to:1000
                }
            }
        }




        // Targets
        Rectangle{
            width: parent.width + 20
            height: 70
            color: "#f9f9f9"
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: sensors.bottom
            anchors.topMargin: 1

            Row
            {
                width: parent.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2

                Label{
                    text: "Lower (mbar)"
                    width: 90
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox {
                    id: lowerPumpSetPoint
                    width: 120
                    value: SystemValuesManager.pumpState[popup.pumpId + "_lower_set_point"]
                    from:1
                    to:800
                }

                Item{
                    width: 20
                    height: 50
                    Rectangle{
                        width:2
                        height: 50
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#f1f1f1"
                    }
                }

                Label{
                    text: "Upper (mbar)"
                    width: 90
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox {
                    id: pumpUpperSetPointState
                    width: 120
                    value: {
                        if(lowerPumpSetPoint.value + 50 > SystemValuesManager.pumpState[popup.pumpId + "_upper_set_point"])
                            return lowerPumpSetPoint.value + 50
                        return SystemValuesManager.pumpState[popup.pumpId + "_upper_set_point"]
                    }
                    from: lowerPumpSetPoint.value + 50
                    to:900
                }

                Item{
                    width: 20
                    height: 50
                    Rectangle{
                        width:2
                        height: 50
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#f1f1f1"
                    }
                }

                Label{
                    text: "Warm up (min)"
                    width: 100
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox {
                    id: pumpWarmupState
                    width: 130
                    value: SystemValuesManager.pumpState[popup.pumpId + "_warm_up"]
                    from:1
                    to:100
                }
            }
        }




        Rectangle{
            id: commands

            width: popup.width
            height: 70

            anchors.bottom: parent.bottom
            anchors.bottomMargin: -12
            anchors.left: parent.left
            anchors.leftMargin: -12

            color: "#eaeaea"

            radius: 4

            // Cover top radius
            Rectangle{
                width: parent.width
                height: 4
                anchors.top: parent.top
                anchors.left: parent.left
                color: "#eaeaea"
            }

            Button{
                id: saveButton
                Materials.Material.accent: Materials.Material.Pink
                Materials.Material.theme: Materials.Material.Light
                highlighted: true
                anchors.right: closeButton.left
                anchors.rightMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "Save"
                onClicked: {
                    // Save values
                    SettingsUpdaterManager.updatePumpSettings(popup.pumpId, pumpAutoState.currentIndex, pumpManualState.currentIndex,
                                                              pumpAlarmState.value, pumpAlarmTimeState.value,
                                                              lowerPumpSetPoint.value, pumpUpperSetPointState.value, pumpWarmupState.value);
                    // Close popup
                    settingsLoaderAliase.active = false
                    settingsLoaderAliase.focus = false
                    settingsLoaderAliase.source = ""
                    settingsLoaderAliase.visible = false
                }
            }
            Button{
                id: closeButton
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "Close"
                onClicked: {
                    // Close popup
                    settingsLoaderAliase.active = false
                    settingsLoaderAliase.focus = false
                    settingsLoaderAliase.source = ""
                    settingsLoaderAliase.visible = false
                }
            }
        }
    }
}
