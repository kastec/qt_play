//import QtQuick 2.0
//import Sailfish.Silica 1.0
//import QtQuick.Layouts 1.1
//import CrewTablet 1.0
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import AppQtTest12 1.0

Item {

    property var viewModel

    Component.onCompleted: {
        viewModel.addPainter("navigation", planeNavArea)
    }

    //    function setMapPosition(pos) {
    //        planeNavArea.setMapPosition(pos)
    //    }
    Rectangle {
        color: "#D9E6ED"
        anchors.fill: parent
        radius: 32

        PaintArea {
            id: planeNavArea
            anchors.fill: parent
        }
    }
}
