import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.VirtualKeyboard 2.2
import QtQuick.VirtualKeyboard.Settings 2.2
import QtQuick.Controls.Material 2.2 as Materials


Popup {

    id: popup

    property int set: 1

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

        if(popup.set == 1)
        {
            return -23;
        }

        return (-23 - (131 * (popup.set - 1)))
    }

    y: {
        if(popup.settingy != 0)
            return popup.settingy

        return -100
    }
    closePolicy: Popup.NoAutoClose
    modal: true
    visible: false;

    contentItem: Item{
        id:item
        Text{
            id: title
            text: "Barrel settings (set " + popup.set + ")"
            anchors.left: parent.Left
            anchors.top: parent.top
            anchors.topMargin: 0
            width: parent.width
            height: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize : 15
        }

        // Enableds
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
                    id: barrelAutoState
                    width:150
                    model: ["Disable", "Enable"]
                    currentIndex: (SystemValuesManager.barrelState[popup.set + "_auto"]) ? 1 :0
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
                    id: barrelManualState
                    width:150
                    model: ["Disable", "Enable"]
                    currentIndex: (SystemValuesManager.barrelState[popup.set + "_manual"]) ? 1 : 0
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
                    text: "Alarm set point (mbar)"
                    width: 170
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox {
                    id: barrelAlarmSetPointState
                    width: 150
                    value: SystemValuesManager.barrelState[popup.set + "_alarm_pressure"]
                    from:1
                    to:500
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
                    text: "Alarm time (sec)"
                    width: 140
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox {
                    id: barrelAlarmTimeState
                    width: 180
                    value: SystemValuesManager.barrelState[popup.set + "_alarm_time"]
                    from:1
                    to: 1000
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
                    text: "Lower set point (mbar)"
                    width: 170
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox {
                    id: barrelLowerSetPoint
                    width: 150
                    value: SystemValuesManager.barrelState[popup.set + "_lower_set_point"]
                    from: 1
                    to: 800
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
                    text: "Upper set point (mbar)"
                    width: 170
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox {
                    id: barrelUpperSetPointstate
                    width: 150
                    value: {
                        if(barrelLowerSetPoint.value + 50 > SystemValuesManager.barrelState[popup.set + "_upper_set_point"])
                            return barrelLowerSetPoint.value + 50
                        return SystemValuesManager.barrelState[popup.set + "_upper_set_point"]
                    }
                    from: barrelLowerSetPoint.value + 50
                    to: 900
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
                    // Save info
                    SettingsUpdaterManager.updateBarrelSettings(popup.set, barrelAutoState.currentIndex, barrelManualState.currentIndex,
                                                                barrelAlarmSetPointState.value, barrelAlarmTimeState.value,
                                                                barrelLowerSetPoint.value, barrelUpperSetPointstate.value);

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
