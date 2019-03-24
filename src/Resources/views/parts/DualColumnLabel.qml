import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2 as Materials

Column {

    id: columnLabel

    property string topLabel: ""
    property string bottomLabel: ""

    anchors.top: parent.top
    anchors.topMargin: 8

    Label{
        text: columnLabel.topLabel
        width: 55
        Materials.Material.accent: Materials.Material.foreground
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter

        font.pointSize: 11
    }
    Label{
        text: columnLabel.bottomLabel
        width: 45
        Materials.Material.accent: Materials.Material.foreground
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 9
    }
}
