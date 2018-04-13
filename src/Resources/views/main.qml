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

    initialPage: FluidControls.TabbedPage {
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
                onTriggered: console.log("Show help dialog")
            }
        ]

        FluidControls.Tab {
            title: qsTr("System Control")

            Label {
                text: qsTr("System Control")
                anchors.centerIn: parent
            }
        }

        FluidControls.Tab {
            title: qsTr("Vacuum Data")

            Label {
                text: qsTr("Vacuum Data")
                anchors.centerIn: parent
            }

        }

        FluidControls.Tab {
            title: qsTr("Switch Pump")

            Label {
                text: qsTr("Switch Pump")
                anchors.centerIn: parent
            }
        }
    }
}
