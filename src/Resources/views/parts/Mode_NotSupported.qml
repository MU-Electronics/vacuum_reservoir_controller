import QtQuick 2.0
import QtQuick.Controls 2.0
import Fluid.Controls 1.0

Item {
    id: item

    property string featureName: "Currently Not Supported"

    Card {
        id: card
        anchors.centerIn: parent
        width: 400
        height: 400

        Image {
            id: picture
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
            height: 200
            source: "../../assets/images/CommingSoon.jpg"

            BusyIndicator {
                anchors.centerIn: parent
                visible: picture.status !== Image.Ready
            }
        }

        Column {
            id: column
            anchors {
                left: parent.left
                top: picture.bottom
                right: parent.right
                margins: Units.smallSpacing * 2
            }
            spacing: Units.smallSpacing * 2

            TitleLabel {
                text: item.featureName
            }

            BodyLabel {
                text: qsTr("<p>This feature has yet to be impliment into the program " +
                           "but is planned in the near future.</p>" +
                           "<p>If you require is feature please speak to Electronic Services. " +
                           "<br/>Phone: 54641 <br/> Email: chemelec@listserv.manchester.ac.uk </p>")
                wrapMode: Text.WordWrap
                width: parent.width
            }
        }
    }
}
