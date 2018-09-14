import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.VirtualKeyboard 2.2
import QtQuick.VirtualKeyboard.Settings 2.2


Popup {

    property int set: 1

    // Settings location
    property int settingx: 0
    property int settingy: 0

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

        return -45
    }
    closePolicy: Popup.NoAutoClose
    modal: true
    visible: false;


    Text{
        id: title
        text: "Set pressure sensor settings for set " + popup.set
        anchors.left: parent.Left
        anchors.top: parent.top
        anchors.topMargin: 10
        width: parent.width
        height: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize : 15
    }

    TextField {
        id: contentContainer
        width: parent.width
        placeholderText: "Set " + popup.set + " comment..."
        anchors.left: title.left
        anchors.top: title.bottom
        anchors.topMargin: 10
        color: "#151515";
    }

    Button{
        id: saveButton
        anchors.left: title.left
        anchors.leftMargin: 10
        anchors.top: contentContainer.bottom
        anchors.topMargin: 20
        text: "Save"
        onClicked: {
            settingsloader.active = false
            settingsloader.focus = false
            settingsloader.source = ""
            settingsloader.visible = false
        }
    }
    Button{
        id: closeButton
        anchors.left: saveButton.right
        anchors.leftMargin: 10
        anchors.top: contentContainer.bottom
        anchors.topMargin: 20
        text: "Close"
        onClicked: {
            settingsloader.active = false
            settingsloader.focus = false
            settingsloader.source = ""
            settingsloader.visible = false
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
