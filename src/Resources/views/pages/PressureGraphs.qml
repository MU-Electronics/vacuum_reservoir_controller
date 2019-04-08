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

        width: 80
        height: 400

        Material.background: Material.color(Material.Grey, Material.Shade300)
        Material.elevation: 1

        ListView
        {
            id: listLogPane

            anchors.fill: parent
            currentIndex: 1

            model: [
                { name: "P1",  enabled: SystemValuesManager.barrelState["1_enabled"] },
                { name: "P2",  enabled: SystemValuesManager.barrelState["2_enabled"] },
                { name: "P3",  enabled: SystemValuesManager.barrelState["3_enabled"] },
                { name: "P4",  enabled: SystemValuesManager.barrelState["4_enabled"] },
                { name: "P5",  enabled: SystemValuesManager.barrelState["5_enabled"] },
                { name: "P6",  enabled: SystemValuesManager.barrelState["6_enabled"] },
                { name: "P7",  enabled: SystemValuesManager.pumpState["1_enabled"] },
                { name: "P8",  enabled: SystemValuesManager.pumpState["2_enabled"] },
            ]

            delegate: FluidControls.ListItem{
                text: modelData["name"]
                highlighted: ListView.isCurrentItem
                enabled: modelData["enabled"]
                onClicked: {

                }
            }

            //ScrollBar.vertical: ScrollBar {}
        }
    }

    GuageOneGraph
    {
        anchors.left: logList.right
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: - 70

        width: 800 - 10 - logList.width
        height: 470
    }

//    Item {
//        anchors.left: logList.right
//        anchors.leftMargin: 10
//        anchors.top: parent.top
//        anchors.topMargin: 10

//        width: 800 - 10 - logList.width
//        height: 400

//        ListView
//        {
//            id: logViewer

//            anchors.fill: parent

//            model: LogsManager.logData

//            header: FluidControls.Subheader {
//               text: "Viewing Log: " + LogsManager.viewingLog
//            }

//            delegate: FluidControls.ListItem{
//                text: modelData["type"] + " at " + modelData["timestamp"] + " in " + modelData["category"]
//                subText: modelData["message"]
//                height: 88
//                maximumLineCount: 4
//            }

//            ScrollBar.vertical: ScrollBar {}
//        }
//    }
}


