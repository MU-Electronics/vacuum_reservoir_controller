import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0
import Fluid.Controls 1.0 as FluidControls
import Fluid.Effects 1.0
import QtQuick.Controls.Material 2.2 as Materials


import "pages"
import "template"


FluidControls.ApplicationWindow {

    id: window

    // Ensure there is no frame around the program
    //flags: Qt.FramelessWindowHint

    // Ensure the window is visable
    visible: true

    // Ensure a minimum width and height
    minimumWidth: 800
    minimumHeight: 480

    title: qsTr("Electronic Services: Vacuum Reservoir Controller")

    // Set the app bar properties
    appBar.maxActionCount: 2

    BusyIndicator {
        id: tabLoading
        running: ((defaultTab.status == Loader.Loading) || (pressureGraphsTab.status == Loader.Loading) || (systemLogsTab == Loader.Loading)) ? true : false
        width: 80
        height: 80
        x: 350
        y: 180
        visible: ((defaultTab.status == Loader.Loading) || (pressureGraphsTab.status == Loader.Loading) || (systemLogsTab == Loader.Loading)) ? true : false
    }

    // Remote control alert
    Item{
        x: 650
        y: -38
        z: 50
        width: 100
        height: 40
        visible: SystemValuesManager.controlState["remote"]
        Button {
           text: "Remote Control"
           highlighted: true
           enabled: false

           contentItem: Text {
               text: parent.text
               font: parent.font
               color: "#ffffff"
           }

           background: Rectangle {
               implicitWidth: 100
               implicitHeight: 40
               opacity: 1
               radius: 2
               color: Materials.Material.color(Materials.Material.Pink, Materials.Material.ShadeA400)
           }
       }
    }

    // Control type minus remote
    Item {
        x: 650
        y: -48
        z: 50
        visible: !SystemValuesManager.controlState["remote"]
        Switch {
            id: modeSwitch
            text: qsTr("Automated")
            checked: SystemValuesManager.controlState["manual_auto"]
            Materials.Material.accent: Materials.Material.Pink
            Materials.Material.theme: Materials.Material.Dark

            Label{
                Materials.Material.theme: Materials.Material.light
                text: "Manual"
                anchors.right: parent.left
                anchors.rightMargin: 2
                anchors.top: parent.top
                anchors.topMargin: 14
                //renderType : parent.renderType
            }

            onClicked: {
                SystemValuesManager.setControlState("manual_auto", modeSwitch.checked);
            }
        }
        CheckBox{
            id: touchControlSwitch
            checked: SystemValuesManager.controlState["touch_control"]
            visible: (modeSwitch.position) ? false : true;
            Materials.Material.accent: Materials.Material.Pink
            Materials.Material.theme: Materials.Material.Dark
            text: "Touch control"
            anchors.top: modeSwitch.bottom
            anchors.topMargin: -10
            anchors.left: modeSwitch.left
            anchors.leftMargin: -56

            onClicked: {
                SystemValuesManager.setControlState("touch_control", touchControlSwitch.checked);
            }
        }
    }

    initialPage: FluidControls.TabbedPage {
        id: mainTab
        title: qsTr("Electronic Services: Vacuum Reservoir Controllers")

        FluidControls.Tab {
            title: qsTr("System Control")

            Loader {
                id: defaultTab
                width: parent.width
                source: "pages/Default.qml" // Default
                active: (mainTab.currentIndex === 0) ? true : false
                asynchronous: true
                visible: (status == Loader.Ready && mainTab.currentIndex == 0) ? true : false
            }
        }

        FluidControls.Tab {
            title: qsTr("Pressure Graphs")

            Loader {
                id: pressureGraphsTab
                width: parent.width
                source: "pages/PressureGraphs.qml"
                active: (mainTab.currentIndex === 1) ? true : false
                asynchronous: true
                visible: (status == Loader.Ready && mainTab.currentIndex == 1) ? true : false
            }

        }

        FluidControls.Tab {
            title: qsTr("System logs")

            Loader {
                id: systemLogsTab
                width: parent.width
                source: "pages/SystemLogs.qml" // SystemLogs
                active: (mainTab.currentIndex === 2) ? true : false
                asynchronous: true
                visible: (status == Loader.Ready && mainTab.currentIndex == 2) ? true : false
            }
        }
    }
}

