import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Rectangle
{
    id: comment

    // Comment name
    property string info: "none"

    width: 75
    height: 52

    color: "#dadada"

    border.width: 2
    border.color: "#afafae"

    Rectangle{
        id: commenttitle
        width: parent.width
        height: 20
        color: "#aeaead"
        radius: 2

        Text{
            text: "Comment"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "#e2e2e2"
            height: parent.height
            width: parent.width
            font.weight: Font.DemiBold
        }
    }

    Text{
        text: comment.info
        anchors.top: commenttitle.bottom
        height: parent.height - commenttitle.height
        width: parent.width
        padding: 3
        wrapMode: Text.WordWrap
        color: "#585858"
    }
}
