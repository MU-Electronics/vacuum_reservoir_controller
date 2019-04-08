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

        if(popup.set == 1)
        {
            return -23;
        }

        return (-23 - (131 * (popup.set - 1)))
    }

    y: {
        if(popup.settingy != 0)
            return popup.settingy

        return -200
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
                    currentIndex: (SystemValuesManager.barrelSettings[popup.set + "_auto"]) ? 1 :0
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
                    currentIndex: (SystemValuesManager.barrelSettings[popup.set + "_manual"]) ? 1 : 0
                }
            }
        }


        // Targets
        Rectangle{
            id: targets
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
                    text: "Lower set point (mbar)"
                    width: 170
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                DecimalSpinner{
                    id: barrelLowerSetPoint
                    width: 150
                    from: 1
                    to: 800 * factor
                    value: SystemValuesManager.barrelSettings[popup.set + "_lower_set_point"] * factor
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

                DecimalSpinner{
                    id: barrelUpperSetPointstate
                    width: 150
                    to: 900 * factor
                    from: barrelLowerSetPoint.value + 1
                    value: {
                        if(barrelLowerSetPoint.value + 0.1 > (SystemValuesManager.barrelSettings[popup.set + "_upper_set_point"] * factor))
                            return (barrelLowerSetPoint.value + 0.1)
                        return (SystemValuesManager.barrelSettings[popup.set + "_upper_set_point"]) * factor
                    }
                }
            }
        }



        // Leak detection
        Rectangle{
            id: leaks
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
                    id: barrelLeakDetection
                    width:120
                    model: ["Disable", "Enable"]
                    currentIndex: (SystemValuesManager.barrelSettings[popup.set + "_leak_detection"]) ? 1 : 0
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
                    id: barrelLeakPeriod
                    width: 130
                    value: SystemValuesManager.barrelSettings[popup.set + "_leak_period"] / 1000
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
                    id: barrelLeakFall
                    width: 140
                    from: 1
                    to: 999 * factor
                    value: SystemValuesManager.barrelSettings[popup.set + "_leak_max"] * factor
                }
            }
        }







        // Pumping settings
        Rectangle{
            id: pumpingLeakDelays
            width: parent.width + 20
            height: 70
            color: "#f2f2f2"
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
                    text: "Barrel leak delay (sec)"
                    width: 170
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox{
                    id: barrelLeakDelay
                    width: 150
                    from: 1
                    to: 999
                    value: SystemValuesManager.barrelSettings[popup.set + "_leak_delay"] / 1000
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
                    text: "Pumping leak delay (sec)"
                    width: 200
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox{
                    id: barrelPumpLeakDelay
                    width: 150
                    from: 1
                    to: 999
                    value: SystemValuesManager.barrelSettings[popup.set + "_barrel_delay"] / 1000
                }
            }
        }






        // Pumping settings
        Rectangle{
            id: pumpTimings
            width: parent.width + 20
            height: 70
            color: "#f9f9f9"
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: pumpingLeakDelays.bottom
            anchors.topMargin: 1

            Row
            {
                width: parent.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2

                Label{
                    text: "Pumping Time (sec)"
                    width: 170
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                SpinBox{
                    id: barrelPumpingTime
                    width: 150
                    from: 10
                    to: 999
                    value: SystemValuesManager.barrelSettings[popup.set + "_pumping_time"] / 1000
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
                    text: "Heavy Load (mbar)"
                    width: 140
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                DecimalSpinner {
                    id: barrelHeavyLoad
                    width: 180
                    value: {
                        //if(barrelLowerSetPoint.value + 0.1 > (SystemValuesManager.barrelSettings[popup.set + "_heavy_load"] * factor))
                        //    return (barrelLowerSetPoint.value + 0.1)
                        return (SystemValuesManager.barrelSettings[popup.set + "_heavy_load"]) * factor
                    }
                    from: 1
                    to: barrelLeakFall.value
                }
            }
        }





        Rectangle{
            id: commands

            width: popup.width
            height: rowHeights

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
                                                                barrelPumpingTime.value, (barrelHeavyLoad.value / barrelHeavyLoad.factor),
                                                                (barrelLowerSetPoint.value / barrelLowerSetPoint.factor), (barrelUpperSetPointstate.value / barrelUpperSetPointstate.factor),
                                                                barrelLeakDetection.currentIndex, (barrelLeakPeriod.value * 1000), (barrelLeakFall.value / barrelLowerSetPoint.factor),
                                                                barrelLeakDelay.value, barrelPumpLeakDelay.value);

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
