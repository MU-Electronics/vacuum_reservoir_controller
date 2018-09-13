import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Fluid.Controls 1.0 as FluidControls
import Fluid.Effects 1.0

import "pages"

FluidControls.ApplicationWindow {

    // Ensure there is no frame around the program
    //flags: Qt.FramelessWindowHint

    // Ensure the window is visable
    visible: true

    // Ensure a minimum width and height
    minimumWidth: 800
    minimumHeight: 480

    title: qsTr("JFET Services: Vacuum Reservoir Controller")

    // Set the app bar properties
    appBar.maxActionCount: 2

    BusyIndicator {
        id: tabLoading
        running: ((defaultTab.status == Loader.Loading) || (pressureGraphsTab.status == Loader.Loading)) ? true : false
        width: 80
        height: 80
        x: 350
        y: 180
        visible: ((defaultTab.status == Loader.Loading) || (pressureGraphsTab.status == Loader.Loading)) ? true : false
    }

    initialPage: FluidControls.TabbedPage {
        id: mainTab
        title: qsTr("JFET Services: Vacuum Reservoir Controllers")

        actions: [
            FluidControls.Action {
                iconName: "action/settings"
                text: qsTr("System Settings")
                toolTip: qsTr("Advanced system settings")
                onTriggered: console.log("Go to settings page")
            },
            FluidControls.Action {
                iconName: "action/help"
                text: qsTr("Help")
                toolTip: qsTr("Help")
                onTriggered: GlobalManager.test()
            }
        ]

        FluidControls.Tab {
            title: qsTr("System Control")

            Loader {
                id: defaultTab
                width: parent.width
                source: "pages/Default.qml"
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

            Label {
                text: qsTr("List logs here")
                anchors.centerIn: parent
            }
        }
    }
}
