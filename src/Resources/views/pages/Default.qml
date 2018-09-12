import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "../parts/rig"


Item{
    width: parent.width
    height: parent.height

    Valve{
        // anchors.top: parent.top
        x: 100
        y: 100
        state: 1
    }

    Valve{
        // anchors.top: parent.top
        x: 200
        y: 100
        state: 2
    }

    Valve{
        // anchors.top: parent.top
        x: 300
        y: 100
        state: 3
    }

    Valve{
        // anchors.top: parent.top
        x: 400
        y: 100
        state: 4
    }
}
