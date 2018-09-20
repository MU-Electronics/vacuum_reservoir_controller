import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0 as Materials
import Fluid.Controls 1.0

Popup {
    id: errors
    width: 400
    height: 200
    x: parent.width / 4
    y: parent.height / 4
    closePolicy: Popup.NoAutoClose
    modal: true
    visible: ErrorsManager.visualErrorSettings["status"]

    contentItem: Item{
        id:item
        Rectangle{
            id: title

            width: errors.width
            height: 40

            anchors.top: parent.top
            anchors.topMargin: -12
            anchors.left: parent.left
            anchors.leftMargin: -12

            color: ErrorsManager.visualErrorSettings["bg_colour"]

            radius: 4

            // Cover top radius
            Rectangle{
                width: parent.width
                height: 4
                anchors.top: parent.bottom
                anchors.left: parent.left
                color: ErrorsManager.visualErrorSettings["bg_colour"]
            }

            Text{
                id: titleText
                text: ErrorsManager.visualErrorSettings["title"]
                color: ErrorsManager.visualErrorSettings["tx_colour"]
                anchors.left: parent.Left
                anchors.top: parent.top
                width: parent.width
                height: 40
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize : 15
            }
        }

        Text{
            id: messageText
            text: ErrorsManager.visualErrorSettings["message"]
            color: ErrorsManager.visualErrorSettings["tx_colour"]
            anchors.left: parent.Left
            anchors.top: title.top
            anchors.topMargin: 20
            width: parent.width
            height: 100
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize : 11
        }

        Rectangle{
            id: commands

            width: errors.width
            height: 70

            anchors.bottom: parent.bottom
            anchors.bottomMargin: -12
            anchors.left: parent.left
            anchors.leftMargin: -12

            color: "#f2f2f2"

            radius: 4

            // Cover top radius
            Rectangle{
                width: parent.width
                height: 4
                anchors.top: parent.top
                anchors.left: parent.left
                color: "#f2f2f2"
            }

            Button{
                id: acceptButton
                Materials.Material.accent: Materials.Material.Pink
                Materials.Material.theme: Materials.Material.Light
                highlighted: true
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "Acknowledge"
                onClicked: {
                    ErrorsManager.acknowledgeError();
                }
            }
        }

    }

}
