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
            return 20;
        }

        return (20 - (131 * (popup.set - 1)))
    }

    y: {
        if(popup.settingy != 0)
            return popup.settingy

        return 20
    }
    closePolicy: Popup.NoAutoClose
    modal: true
    visible: false;

    contentItem: Item{
        Text{
            id: title
            text: "Comment settings (set " + popup.set + ")"
            anchors.left: parent.Left
            anchors.top: parent.top
            anchors.topMargin: 10
            width: parent.width
            height: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize : 15
        }
        Label{
            id: info
            text: "Add a comment to hint where the current set is connected; maximum 25 characters."
            anchors.left: title.Left
            anchors.top: title.top
            anchors.topMargin: 45
            width: parent.width
            Materials.Material.accent: Materials.Material.foreground
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

        }

        TextField {
            id: contentContainer
            width: parent.width
            placeholderText: "Set " + popup.set + " comment..."
            anchors.left: info.left
            anchors.top: info.bottom
            anchors.topMargin: 0
            color: "#151515";
            maximumLength: 25
            text: SystemValuesManager.commentSettings[popup.set + "_value"]
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
                    // Save comment
                    SettingsUpdaterManager.updateCommentSettings(popup.set, contentContainer.text);

                    // Close popup
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
                    // Close popup
                    settingsLoaderAliase.active = false
                    settingsLoaderAliase.focus = false
                    settingsLoaderAliase.source = ""
                    settingsLoaderAliase.visible = false
                }
            }
        }
    }

    Item {
        id: appContainer
        width: 600 // 680
        height: 200
        anchors.left: popup.left
        anchors.bottom: popup.bottom
        anchors.bottomMargin: -100
        //rotation: Screen.width < Screen.height ? 90 : 0

        InputPanel {
            id: inputPanel
            z: 89
            y: appContainer.height + 145

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 50

            states: State {
                name: "visible"
                when: inputPanel.active
                PropertyChanges {
                    target: inputPanel
                    y: (appContainer.height + 145) - inputPanel.height
                }
            }
            transitions: Transition {
                id: inputPanelTransition
                from: ""
                to: "visible"
                reversible: true
                enabled: !VirtualKeyboardSettings.fullScreenMode
                ParallelAnimation {
                    NumberAnimation {
                        properties: "y"
                        duration: 250
                        easing.type: Easing.InOutQuad
                    }
                }
            }
            Binding {
                target: InputContext
                property: "animating"
                value: inputPanelTransition.running
            }
            AutoScroller {}
        }

        Binding {
            target: VirtualKeyboardSettings
            property: "fullScreenMode"
            value: appContainer.height > 0 && (appContainer.width / appContainer.height) > (16.0 / 9.0)
        }
    }



}
