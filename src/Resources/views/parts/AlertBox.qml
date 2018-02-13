import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id: alertBox

    property string textContent: "Text not defined"

    property string type: "Primary"

    state: alertBox.type

    height: alertText.y + alertText.height + 7;

    states: [
        State {
            name: "Danger"
            PropertyChanges { target: container; color: Material.color(Material.Red, Material.Shade800); border.color: Material.color(Material.Red, Material.Shade300)}
        },
        State {
            name: "Warning"
            PropertyChanges { target: container; color: Material.color(Material.Yellow, Material.Shade800); border.color: Material.color(Material.Yellow, Material.Shade300)}
        },
        State {
            name: "Notice"
            PropertyChanges { target: container; color: Material.color(Material.Blue, Material.Shade800); border.color: Material.color(Material.Blue, Material.Shade300)}
        },
        State {
            name: "Success"
            PropertyChanges { target: container; color: Material.color(Material.Green, Material.Shade800); border.color: Material.color(Material.Green, Material.Shade300)}
        },
        State {
            name: "Primary"
            PropertyChanges { target: container; color: Material.color(Material.Grey, Material.Shade800); border.color: Material.color(Material.Grey, Material.Shade300)}
        }
    ]

    Rectangle
    {
        id: container

        color: Material.color(Material.Red, Material.Shade800)

        border.color: Material.color(Material.Red, Material.Shade300)
        border.width: 1

        //height: parent.height
        height: alertText.y + alertText.height + 7;
        width: parent.width

        Text {
            id: alertText

            text: alertBox.textContent

            width: parent.width - 20
            wrapMode: Text.WordWrap

            color: Material.color(Material.Grey, Material.Shade100)

            font.pixelSize: 13
            font.weight: Font.Bold

            anchors.top: parent.top
            anchors.topMargin: 7
            anchors.left: parent.left
            anchors.leftMargin: 10
        }
    }

}
