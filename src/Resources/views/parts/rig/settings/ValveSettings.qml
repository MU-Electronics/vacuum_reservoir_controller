import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.VirtualKeyboard 2.2
import QtQuick.VirtualKeyboard.Settings 2.2
import QtQuick.Controls.Material 2.2 as Materials


Popup {

    property int set: 1

    // Settings location
    property int settingx: 0
    property int settingy: 0

    property var settingsLoaderAliase: 0

    id: popup
    width: 720
    height: 320
    //contentWidth: view.implicitWidth
    //contentHeight: view.implicitHeight
    x: {
        if(popup.settingx != 0)
            return popup.settingx

        if(popup.set == 1)
        {
            return -35;
        }

        return (-35 - (131 * (popup.set - 1)))
    }

    y: {
        if(popup.settingy != 0)
            return popup.settingy

        return -193
    }
    closePolicy: Popup.NoAutoClose
    modal: true
    visible: false;


    contentItem: Item
    {
        id:item

        Text{
            id: title
            text: "Valve settings (set " + popup.set + ")"
            anchors.left: parent.Left
            anchors.top: parent.top
            anchors.topMargin: 0
            width: parent.width
            height: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize : 15
        }



        Rectangle{
            id: sensors
            width: parent.width + 20
            height: 70
            color: "#f9f9f9"
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: title.bottom
            anchors.topMargin: 20

            Row
            {
                width: parent.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2

                Label{
                    text: "Valve IO: Group " + popup.set
                    width: 300
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

                Item{
                    width: 50
                    height: 50
                    Rectangle{
                        width:2
                        height: 50
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#f9f9f9"
                    }
                }

                Label{
                    text: "Valve Type: Edwards LCPV25EKA"
                    width: 300
                    Materials.Material.accent: Materials.Material.foreground
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    font.pointSize: 11
                }

            }
        }



        Rectangle{
            id: commands

            width: popup.width
            height: 70

            anchors.bottom: parent.bottom
            anchors.bottomMargin: -12
            anchors.left: parent.left
            anchors.leftMargin: -12

            color: "#eaeaea"

            radius: 4

            // Cover top radius
            Rectangle{
                width: parent.width
                height: 4
                anchors.top: parent.top
                anchors.left: parent.left
                color: "#eaeaea"
            }

            Button{
                id: saveButton
                Materials.Material.accent: Materials.Material.Pink
                Materials.Material.theme: Materials.Material.Light
                highlighted: true
                anchors.right: closeButton.left
                anchors.rightMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "Save"
                onClicked: {
                    settingsLoaderAliase.active = false
                    settingsLoaderAliase.focus = false
                    settingsLoaderAliase.source = ""
                    settingsLoaderAliase.visible = false
                }
            }
            Button{
                id: closeButton
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "Close"
                onClicked: {
                    settingsLoaderAliase.active = false
                    settingsLoaderAliase.focus = false
                    settingsLoaderAliase.source = ""
                    settingsLoaderAliase.visible = false
                }
            }
        }

    }
}
