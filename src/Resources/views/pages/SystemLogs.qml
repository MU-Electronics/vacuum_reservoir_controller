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

        width: 200
        height: 400

        Material.background: Material.color(Material.Grey, Material.Shade300)
        Material.elevation: 1

        ListView
        {
            id: listLogPane

            anchors.fill: parent
            currentIndex: 0

            model: ListModel {
                ListElement { title: qsTr("Current"); }
                ListElement { title: qsTr("10/09/2018"); }
            }

            header: FluidControls.Subheader {
               text: qsTr("Select Logs")
            }

            delegate: FluidControls.ListItem{
                text: model.title
                highlighted: ListView.isCurrentItem

            }

            ScrollBar.vertical: ScrollBar {}
        }
    }

    Item {
        anchors.left: logList.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        Row {
            Text {
                text: "log entry here"
            }
        }
    }
}


