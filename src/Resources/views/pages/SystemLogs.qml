import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import Fluid.Controls 1.0 as FluidControls

import "parts"

Item
{
    anchors.fill: parent

    Pane
    {
        id: logList

        anchors {
            left: parent.left
            top: parent.top
            leftMargin: 0
            topMargin: -5
        }

        width: 230
        height: 400

        Material.background: Material.color(Material.Grey, Material.Shade300)
        Material.elevation: 1

        ListView
        {
            id: listLogPane

            anchors.fill: parent
            currentIndex: LogsManager.currentLog

            model: LogsManager.logList

            header: FluidControls.Subheader {
               text: qsTr("Select Log")
            }

            delegate: FluidControls.ListItem{
                text: modelData["name"]
                highlighted: ListView.isCurrentItem
                onClicked: {
                    LogsManager.selectLog(modelData["filename"])
                    LogsManager.currentLog = index;
                }
            }

            ScrollBar.vertical: ScrollBar {}
        }
    }

    Item {
        anchors.left: logList.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        width: 800 - 10 - logList.width
        height: 400

        ListView
        {
            id: logViewer

            anchors.fill: parent

            model: LogsManager.logData

            header: FluidControls.Subheader {
               text: "Viewing Log: " + LogsManager.viewingLog
            }

            delegate: FluidControls.ListItem{
                text: modelData["type"] + " at " + modelData["timestamp"] + " in " + modelData["category"]
                subText: modelData["message"]
                height: 95
                maximumLineCount: 4
            }

            ScrollBar.vertical: ScrollBar {}
        }
    }
}


