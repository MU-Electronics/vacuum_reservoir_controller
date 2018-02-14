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
    minimumWidth: 1280
    minimumHeight: 725

    title: qsTr("Electronic Services: Vacuum Reservoir Controller")

    // Set the app bar properties
    appBar.maxActionCount: 1



    initialPage: FluidControls.TabbedPage {
        title: qsTr("Tabbed Page")

        actions: [
            FluidControls.Action {
                iconName: "content/add"
                text: qsTr("Add content")
                toolTip: qsTr("Add content")
                onTriggered: console.log("Example action...")
            }
        ]

        FluidControls.Tab {
            title: qsTr("First")

            Label {
                text: qsTr("First page")
                anchors.centerIn: parent
            }
        }

        FluidControls.Tab {
            title: qsTr("Second")

            Label {
                text: qsTr("Second page")
                anchors.centerIn: parent
            }
        }
    }
}
