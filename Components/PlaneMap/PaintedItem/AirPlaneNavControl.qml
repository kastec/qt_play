//import QtQuick 2.0
//import Sailfish.Silica 1.0
//import QtQuick.Layouts 1.1
//import CrewTablet 1.0
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import AppQtTest12 1.0

Item {
    id: rootControl

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
    }
    Item {
        anchors.fill: parent
        anchors.margins: 20

        PaintArea {
            id: planeNavArea
            anchors.fill: parent
        }

        MouseArea {
            anchors.fill: parent

            property real ix
            property real iy

            property date pressTime
            property var heldTimeMsec
            property real pressX
            property real pressY
            property real moveDistance

            //        enabled: !pa.isDragging
            onPositionChanged: {
                if (Math.abs(ix - mouseX) > 1.0 || Math.abs(iy - mouseY) > 1.0) {
                    //                    airplaneDrawer.moveBy(ix - mouseX, iy - mouseY)
                    //                viewModel.moveNavBy(ix - mouseX, iy - mouseY)
                    ix = mouseX
                    iy = mouseY
                }
            }

            onPressed: {
                ix = mouseX
                iy = mouseY

                pressTime = new Date()
                pressX = mouseX
                pressY = mouseY

                viewModel.setNavPos(ix - x, iy - y)
            }

            onReleased: {
                heldTimeMsec = new Date().getTime() - pressTime.getTime()
                moveDistance = Math.sqrt(Math.pow((mouseX - pressX), 2) + Math.pow((mouseY - pressY), 2))
            }

            onClicked: {
                if (heldTimeMsec > 600 || moveDistance > 10) {
                    return
                }
            }
        } // MouseArea ( inside PinchArea)
    }
}
