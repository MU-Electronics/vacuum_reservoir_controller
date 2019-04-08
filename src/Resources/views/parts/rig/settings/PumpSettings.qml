import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.VirtualKeyboard 2.2
import QtQuick.VirtualKeyboard.Settings 2.2
import QtQuick.Controls.Material 2.2 as Materials


import '../../'

Popup {

    id: popup

    property int set: 1

    property int pumpId: popup.set - 6

    // Settings location
    property int settingx: 0
    property int settingy: 0

    property var settingsLoaderAliase: 0

    property int rowHeights: 65

    width: 720
    height: 440
    //contentWidth: view.implicitWidth
    //contentHeight: view.implicitHeight
    x: {
        if(popup.settingx != 0)
            return popup.settingx

        return (-685)
    }

    y: {
        if(popup.settingy != 0)
            return popup.settingy

        return -290
        //return -390
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
            height: popup.rowHeights
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
                    currentIndex: (SystemValuesManager.pumpSettings[popup.pumpId + "_auto"]) ? 1 : 0
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
                    currentIndex: (SystemValuesManager.pumpSettings[popup.pumpId + "_manual"]) ? 1 : 0
                }
            }
        }





        // Targets
        Rectangle{
            id: targets
            width: parent.width + 20
            height: popup.rowHeights
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

                DualColumnLabel{
                    topLabel: "Lower"
                    bottomLabel: "(mbar)"
                }

                DecimalSpinner{
                    id: lowerPumpSetPoint
                    width: 170
                    from: 0
                    to: 800 * factor
                    value: SystemValuesManager.pumpSettings[popup.pumpId + "_lower_set_point"] * factor
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

                DualColumnLabel{
                    topLabel: "Upper"
                    bottomLabel: "(mbar)"
                }

                DecimalSpinner{
                    id: pumpUpperSetPointState
                    from: lowerPumpSetPoint.value + 0.1
                    width: 170
                    value: {
                        if(lowerPumpSetPoint.value + 50 > (SystemValuesManager.pumpSettings[popup.pumpId + "_upper_set_point"] * factor))
                            return (lowerPumpSetPoint.value + 50)
                        return (SystemValuesManager.pumpSettings[popup.pumpId + "_upper_set_point"]) * factor
                    }
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

                DualColumnLabel{
                    topLabel: "Warm Up"
                    bottomLabel: "(min)"
                }

                SpinBox {
                    id: pumpWarmupState
                    width: 130
                    value: SystemValuesManager.pumpSettings[popup.pumpId + "_warm_up"]
                    from:1
                    to:100
                }
            }
        }



        // Timings
        Rectangle{
            id: pumpingTimings
            width: parent.width + 20
            height: popup.rowHeights
            color: "#f9f9f9"
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: targets.bottom
            anchors.topMargin: 1

            Row
            {
                width: parent.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2


                DualColumnLabel{
                    width: 165
                    topLabel: "Max Pump -> Valve"
                    bottomLabel: "(min)"
                }


                SpinBox {
                    id: pumpMaxPumpValveTime
                    width: 160
                    value: SystemValuesManager.pumpSettings[popup.pumpId + "_pump_void"]
                    from:0
                    to:1000
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

                DualColumnLabel{
                    width: 165
                    topLabel: "Max Pump -> Barrels"
                    bottomLabel: "(min)"
                }

                SpinBox {
                    id: pumpMaxPumpBarrelsTime
                    width: 160
                    value: SystemValuesManager.pumpSettings[popup.pumpId + "_pump_manifold_void"]
                    from:0
                    to:1000
                }
            }
        }


        // Leak detection
        Rectangle{
            id: leaks
            width: parent.width + 20
            height: popup.rowHeights
            color: "#f2f2f2"
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: pumpingTimings.bottom
            anchors.topMargin: 1

            Row
            {
                width: parent.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2

                Label{
                    text: "Leak Detection"
                    width: 120
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                ComboBox {
                    id: pumpLeakDetection
                    width:120
                    model: ["Disable", "Enable"]
                    currentIndex: (SystemValuesManager.pumpSettings[popup.pumpId + "_leak_detection"]) ? 1 : 0
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

                DualColumnLabel{
                    topLabel: "Period"
                    bottomLabel: "(secs)"
                }

                SpinBox {
                    id: pumpLeakPeriod
                    width: 130
                    value: SystemValuesManager.pumpSettings[popup.pumpId + "_leak_period"] / 1000
                    from:1
                    to:999
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

                DualColumnLabel{
                    topLabel: "Max Fall"
                    bottomLabel: "(mbar)"
                }

                DecimalSpinner{
                    id: pumpLeakFall
                    width: 140
                    from: 1
                    to: 999 * factor
                    value: SystemValuesManager.pumpSettings[popup.pumpId + "_leak_max"] * factor
                }
            }
        }






        // More leak settings
        Rectangle{
            id: leakExtra
            width: parent.width + 20
            height: popup.rowHeights
            color: "#f9f9f9"
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: leaks.bottom
            anchors.topMargin: 1

            Row
            {
                width: parent.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2


                Label{
                    text: "Manifold leak delay (secs)"
                    width: 190
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox{
                    id: pumpManiFoldLeak
                    width: 120
                    from: 1
                    to: 99
                    value: SystemValuesManager.pumpSettings[popup.pumpId + "_manifold_delay"] / 1000
                }

                /*Item{
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
                    value: SystemValuesManager.pumpSettings[popup.pumpId + "_alarm_time"]
                    from:0
                    to:1000
                }*/
            }
        }






        // Save / Cancel
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
                                                              pumpManiFoldLeak.value,
                                                              (lowerPumpSetPoint.value / lowerPumpSetPoint.factor), (pumpUpperSetPointState.value/ pumpUpperSetPointState.factor), pumpWarmupState.value,
                                                              pumpMaxPumpValveTime.value, pumpMaxPumpBarrelsTime.value,
                                                              pumpLeakDetection.currentIndex, (pumpLeakPeriod.value*1000), (pumpLeakFall.value / pumpLeakFall.factor));

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
