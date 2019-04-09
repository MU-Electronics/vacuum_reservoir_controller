import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import Fluid.Controls 1.0 as FluidControls

import "parts"

Item
{
    id: pressureGraphsContainer

    anchors.fill: parent

    property int toLoad: 1

    Pane
    {
        id: logList

        anchors {
            left: parent.left
            top: parent.top
            leftMargin: 0
            topMargin: -5
        }

        width: 80
        height: 400

        Material.background: Material.color(Material.Grey, Material.Shade300)
        Material.elevation: 1

        ListView
        {
            id: listLogPane

            anchors.fill: parent
            currentIndex:  pressureGraphsContainer.toLoad - 1;

            Component.onCompleted: {
                if(SystemValuesManager.barrelState["1_enabled"])
                    pressureGraphsContainer.toLoad = 1;
                else if(SystemValuesManager.barrelState["2_enabled"])
                    pressureGraphsContainer.toLoad = 2;
                else if(SystemValuesManager.barrelState["3_enabled"])
                    pressureGraphsContainer.toLoad = 3;
                else if(SystemValuesManager.barrelState["4_enabled"])
                    pressureGraphsContainer.toLoad = 4;
                else if(SystemValuesManager.barrelState["5_enabled"])
                    pressureGraphsContainer.toLoad = 5;
                else if(SystemValuesManager.barrelState["6_enabled"])
                    pressureGraphsContainer.toLoad = 6;
                else if(SystemValuesManager.pumpState["1_enabled"])
                    pressureGraphsContainer.toLoad = 7;
                else if(SystemValuesManager.pumpState["2_enabled"])
                    pressureGraphsContainer.toLoad = 8;
            }

            model: [
                { name: "P1", id: 1, enabled: SystemValuesManager.barrelState["1_enabled"] },
                { name: "P2", id: 2, enabled: SystemValuesManager.barrelState["2_enabled"] },
                { name: "P3", id: 3, enabled: SystemValuesManager.barrelState["3_enabled"] },
                { name: "P4", id: 4, enabled: SystemValuesManager.barrelState["4_enabled"] },
                { name: "P5", id: 5, enabled: SystemValuesManager.barrelState["5_enabled"] },
                { name: "P6", id: 6, enabled: SystemValuesManager.barrelState["6_enabled"] },
                { name: "P7", id: 7, enabled: SystemValuesManager.pumpState["1_enabled"] },
                { name: "P8", id: 8, enabled: SystemValuesManager.pumpState["2_enabled"] }
            ]

            delegate: FluidControls.ListItem{
                text: modelData["name"]
                highlighted: (pressureGraphsContainer.toLoad === modelData["id"]) ? true : false
                enabled: modelData["enabled"]
                onClicked: {
                    pressureGraphsContainer.toLoad = modelData["id"];
                    graphLoader.reload();
                }
            }

            //ScrollBar.vertical: ScrollBar {}
        }
    }

    BusyIndicator {
        id: loadingRing
        running: (graphLoader.status == Loader.Loading) ? true : false
        width: 100
        height: 100
        anchors.top: testingModeTabBar.bottom
        anchors.topMargin: ((window.height-150) / 2) - 50
        anchors.left: testingModeTabBar.left
        anchors.leftMargin: (parent.width / 2) - 50
        visible: (graphLoader.status == Loader.Loading) ? true : false
    }

    Loader {
        id: graphLoader
        source: "parts/GuageGraph.qml"
        // active: (mainTab.currentIndex === 3) ? true : false
        asynchronous: true
        //visible: (status == Loader.Ready && mainTab.currentIndex == 3) ? true : false

        anchors.left: logList.right
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: - 70

        width: 800 - 10 - logList.width
        height: 470

        onLoaded: {
            item.guageId = pressureGraphsContainer.toLoad;
        }

        function reload() {
            source = "";
            source = "parts/GuageGraph.qml";
        }
    }


}


