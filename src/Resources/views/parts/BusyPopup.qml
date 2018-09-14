import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Popup {
    id: popup

    property var status: 0
    property int xpos: 0
    property int ypos: 0

    width: 0
    height: 0
    x: 0
    y: 0
    closePolicy: Popup.NoAutoClose
    modal: true
    visible: (popup.status == Loader.Loading);

    BusyIndicator {
        id: tabLoading
        running: ((popup.status == Loader.Loading)) ? true : false
        width: 80
        height: 80
        x: popup.xpos
        y: popup.ypos
        visible: ((popup.status == Loader.Loading)) ? true : false
    }
}
