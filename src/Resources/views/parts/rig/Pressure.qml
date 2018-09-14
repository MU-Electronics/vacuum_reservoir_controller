import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Rectangle
{
    id: pressure

    width: 75
    height: 36
    radius: 10

    // Chamber set
    property int set: 1

    // State of sensor
    property int state: 1

    // State of sensor
    property double value: 00.00

    // Sensor name
    property string name: "P?"

    // Colours depend on sensor state
    property string colour1: {
        if(pressure.state === 1) // Sensor ok
            return "#dacf07"

        if(pressure.state === 2) // Sensor range
            return "#cb1c29"

        if(pressure.state === 3) // Sensor Error
            return "#c7a028"

        // Disabled id: 4
        return "#c5c4c4"
    }

    // Colours depend on sensor state
    property string colour2: {
        if(pressure.state === 1) // Sensor ok
            return "#c9be06"

        if(pressure.state === 2) // Sensor Range
            return "#bb1a26"

        if(pressure.state === 3) // Sensor Error
            return "#edc64e"

        // Disabled id: 4
        return "#bebdbd"
    }

    // Colours depend on pump state
    property string tcolour: {
        if(pressure.state === 1) // Sensor ok
            return "#ffffff"

        if(pressure.state === 2) // Sensor Range
            return "#ffffff"

        if(pressure.state === 3) // Sensor Error
            return "#ffffff"

        // Disabled id: 4
        return "#ffffff"
    }

    color: pressure.colour1;

    border.width: 2
    border.color: pressure.colour2

    Text{
        id: pressurename
        text: pressure.name
        width: parent.width
        height: 15
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 9
        color: pressure.tcolour
        anchors.left: parent.left;
        anchors.top: parent.top
        anchors.topMargin: 3
        font.weight: Font.DemiBold
    }

    Text{
        text: (pressure.state !== 4) ? pressure.value + " mbar" : "disabled";
        width: parent.width
        height: 15
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 8
        color: pressure.tcolour
        anchors.left: parent.left;
        anchors.top: pressurename.top
        anchors.topMargin: 14
    }
}
