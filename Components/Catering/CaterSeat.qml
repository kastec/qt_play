import QtQuick 2.15
import AppQtTest12 1.0

Rectangle {

    property CaterSeatViewModel caterSeat

    radius: 20
    border.color: "#D1D1D1"
    border.width: 2

    width: 250
    height: 300

    color: "lime"

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        text: caterSeat.name
    }
}
