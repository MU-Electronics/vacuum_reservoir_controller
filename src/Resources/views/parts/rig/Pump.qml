import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Rectangle
{
    id: pump

    // State of pump
    property int state: 1

    // Pump name
    property string name: "Pump ?"

    // Angle
    property bool flip: true

    // Colours depend on pump state
    property string colour1: {
        if(pump.state === 1) // Pump on
            return "#cb1c29"

        if(pump.state === 2) // Pump off
            return "#55965e"

        if(pump.state === 3) // Pump failed
            return "#c7a028"

        // Disabled id: 4
        return "#cac8c8"
    }

    // Colours depend on pump state
    property string colour2: {
        if(pump.state === 1) // Pump on
            return "#f12131"

        if(pump.state === 2) // Pump off
            return "#6bc077"

        if(pump.state === 3) // Pump failed
            return "#edc64e"

        // Disabled id: 4
        return "#dbdbdb"
    }

    // Colours depend on pump state
    property string tcolour: {
        if(pump.state === 1) // Pump on
            return "#ffffff"

        if(pump.state === 2) // Pump off
            return "#ffffff"

        if(pump.state === 3) // Pump failed
            return "#ffffff"

        // Disabled id: 4
        return "#6d6d6d"
    }

    // Colours depend on pump state
    property string tcolour2: {
        if(pump.state === 1) // Pump on
            return "#cb1c29"

        if(pump.state === 2) // Pump off
            return "#55965e"

        if(pump.state === 3) // Pump failed
            return "#c7a028"

        // Disabled id: 4
        return "#797979"
    }

    height: 70
    width: 100
    color: "transparent"

    Rectangle
    {
        id: pumpmotor
        color: pump.colour1
        height: 22
        width: 25

        Text{
            text: {
                if(pump.state === 1)
                    return "On"

                if(pump.state === 2)
                    return "Off"

                if(pump.state === 3)
                    return "ERROR"

                return "Disabled"
            }
            width: 60
            height: 20
            horizontalAlignment: (pump.flip) ? Text.AlignLeft : Text.AlignRight ;
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 8
            color: pump.tcolour2

            // anchors.left: parent.right;
            anchors.left: (pump.flip) ? parent.right : undefined ;
            anchors.leftMargin: 10
            anchors.right: (!pump.flip) ? parent.left : undefined ;
            anchors.rightMargin: 10
        }

        Rectangle
        {
            color: pump.colour2
            height: 2
            width: 25
            anchors.top: parent.top
            anchors.topMargin: 2
        }

        Rectangle
        {
            color: pump.colour2
            height: 2
            width: 25
            anchors.top: parent.top
            anchors.topMargin: 6
        }

        Rectangle
        {
            color: pump.colour2
            height: 2
            width: 25
            anchors.top: parent.top
            anchors.topMargin: 10
        }

        Rectangle
        {
            color: pump.colour2
            height: 2
            width: 25
            anchors.top: parent.top
            anchors.topMargin: 14
        }

        Rectangle
        {
            color: pump.colour2
            height: 2
            width: 25
            anchors.top: parent.top
            anchors.topMargin: 18
        }

    }

    Rectangle
    {
        id: pumpbody
        color: pump.colour1
        height: 30
        width: 100
        anchors.top: pumpmotor.bottom

        anchors.left: (pump.flip) ? pumpmotor.left : undefined ;
        anchors.right: (!pump.flip) ? pumpmotor.right : undefined ;

        Text{
            text: pump.name
            font.weight: Font.DemiBold
            width: parent.width
            height: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 10
            color: pump.tcolour
            anchors.left: parent.left;
        }
    }

    Rectangle
    {
        width: 4
        height: 3
        color: "#a8a8a7"
        anchors.left: pumpbody.left
        anchors.leftMargin: 8
        anchors.top: pumpbody.bottom

        Rectangle
        {
            width: 20
            height: 3
            color: "#a8a8a7"
            anchors.left: parent.left
            anchors.leftMargin: -8
            anchors.top: parent.bottom
        }
    }

    Rectangle
    {
        width: 4
        height: 3
        color: "#a8a8a7"
        anchors.right: pumpbody.right
        anchors.rightMargin: 8
        anchors.top: pumpbody.bottom

        Rectangle
        {
            width: 20
            height: 3
            color: "#a8a8a7"
            anchors.right: parent.left
            anchors.rightMargin: -12
            anchors.top: parent.bottom
        }
    }


}
