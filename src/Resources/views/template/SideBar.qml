import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1
import Fluid.Controls 1.0

Pane {
    id: listPane


    Connections {
        target: GlobalManager
        onCurrentNavigationChanged: {
            // Removed hightling of old page
            if(GlobalManager.currentNavigation == 1)
            {
                connectionTab.highlighted = false
                modeNavigation.currentIndex = -1
            }
        }
    }

    property int currentNavigationWindow: 2

    anchors {
        left: parent.left
        top: parent.top
        bottom: parent.bottom
    }
    width: 200
    padding: 0
    z: 2

    Material.background: "white"
    Material.elevation: 1

    Universal.background: Universal.accent

    ListItem {
        id: connectionTab
        text: qsTr("Seperate Page")
        highlighted: true
        onClicked: {
            // Remove highligh from mode selection
            modeNavigation.currentIndex = -1

            //Add highligh to this tab
            connectionTab.highlighted = true

            // Push the view update
            stackView.replace("qrc:/views/pages/Default.qml")

            // Update current nav
            GlobalManager.currentNavigation = 2;
        }
    }

    ListView {
        id: modeNavigation
        anchors.fill: parent
        anchors.topMargin: 50
        currentIndex: -1
        enabled: ConnectionStatusManager.hardwareConnection["all_connections"];
        model: ListModel {
            ListElement { title: qsTr("Another page"); source: "qrc:/views/pages/Default.qml" }
            ListElement { title: qsTr("And Another"); source: "qrc:/views/pages/Default.qml" }
        }
        header: Subheader {
            text: qsTr("Select Page")
        }
        delegate: ListItem {
                text: model.title
                highlighted: ListView.isCurrentItem

                onClicked:
                {
                    //Remove highligh from connection status
                    connectionTab.highlighted = false

                    // Add highligh to mode
                    modeNavigation.currentIndex = index

                    // Push the view update
                    stackView.replace(model.source)

                    // Update current nav
                    GlobalManager.currentNavigation = 2;

                }

        }
        ScrollBar.vertical: ScrollBar {}
    }
}
