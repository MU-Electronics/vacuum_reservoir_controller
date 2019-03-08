import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import "settings"
import "../"

Rectangle
{
    id: comment

    // Comment name
    property string info: "none"

    // Chamber set
    property int set: 1

    // Settings location
    property int settingx: 0
    property int settingy: 0

    width: 75
    height: 52

    color: "#dadada"

    border.width: 2
    border.color: "#afafae"

    MouseArea {
        anchors.fill: parent
        onClicked: {
            settingsloader.source = "../../parts/rig/settings/CommentSettings.qml"
            settingsloader.active = true
            settingsloader.focus = true
            settingsloader.visible = true
        }
    }

    BusyPopup{
        status: settingsloader.status
        xpos: {
            if(comment.set == 1)
            {
                return 320;
            }

            return (320 - (131 * (comment.set - 1)))
        }
        ypos: 100
    }

    Loader {
        id: settingsloader
        source: ""
        active: false
        asynchronous: true
        visible: false
        focus: false
        onLoaded: {
            item.set = comment.set
            item.settingsLoaderAliase = settingsloader
            if(comment.settingx != 0)
                item.settingx = comment.settingx
            if(comment.settingy != 0)
                item.settingy = comment.settingy
            item.open();
        }
    }

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
