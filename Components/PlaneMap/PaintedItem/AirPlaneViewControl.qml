//import QtQuick 2.0
//import Sailfish.Silica 1.0
//import QtQuick.Layouts 1.1
//import CrewTablet 1.0
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import AppQtTest12 1.0

Item {
    id: airplaneControl

    property alias planeMap: airplaneDrawer.planeMap

    property size dimensions: Qt.size(width, height)

    signal itemClicked(var id)

    signal positionChanged(var pos)

    Component.onCompleted: {
        airplaneDrawer.changeVisibleSize(dimensions)
    }

    //    Text {
    //        id: log
    //        anchors.fill: parent
    //        anchors.margins: 10
    //    }
    function scrollTo(scrollToId) {
        var z = airplaneDrawer.zoom
        if (z < 1.1)
            z = 1.1
        if (z > 2.2)
            z = 2.2
        airplaneDrawer.zoom = z
        airplaneDrawer.position = airplaneDrawer.getMoveToCenterAt(scrollToId, z)
    }

    Column {
        z: 100

        Button {
            text: "FUTURE"
            onClicked: airplaneDrawer.testAsync()
        }

        Button {
            text: "4C"
            onClicked: airplaneControl.scrollTo(text)
        }
        Button {
            text: "18F"
            onClicked: airplaneControl.scrollTo(text)
        }
        Button {
            text: "50B"
            onClicked: airplaneControl.scrollTo(text)
        }

        Button {
            text: "WC6"
            onClicked: airplaneControl.scrollTo(text)
        }

        Button {
            text: airplaneDrawer.avgRenderTime + " / " + airplaneDrawer.renderTime
            //            onClicked: log.text = ""
        }

        Button {
            text: "move 0,0"
            onClicked: airplaneDrawer.position = Qt.point(0, 0)
        }
        Button {
            text: "move 300,300"
            onClicked: airplaneDrawer.position = Qt.point(300, 300)
        }

        Button {
            text: "zoom 0.5"
            onClicked: airplaneDrawer.zoom = 0.5
        }
        Button {
            text: "zoom 1"
            onClicked: airplaneDrawer.zoom = 1.0
        }
        Button {
            text: "zoom 4"
            onClicked: airplaneDrawer.zoom = 4.0
        }
        Button {
            text: "zoom 1, pos -500,-500"
            onClicked: {
                airplaneDrawer.zoom = 1.0
                airplaneDrawer.position = Qt.point(-500, -500)
            }
        }
    }

    onDimensionsChanged: airplaneDrawer.changeVisibleSize(dimensions)

    AirplaneDrawer {
        id: airplaneDrawer

        // planeMap: airplaneViewModel.planeMap
        anchors.fill: parent

        Behavior on zoom {
            NumberAnimation {
                duration: 500
                easing.type: Easing.InOutCubic
            }
        }

        Behavior on position {
            PropertyAnimation {
                duration: 500
                easing.type: Easing.InOutCubic
            }
        }
    } // AirplaneDrawer


    /*
    PinchArea {
        id: pa
        anchors.fill: parent

        property bool isDragging: false
        onPinchFinished: {
            //            log.text += "PinchArea onPinchFinished" + "\n"
            pa.isDragging = false
        }
        onPinchStarted: {
            //            log.text += "PinchArea onPinchStarted" + "\n"
            pa.isDragging = true
        }
        onPinchUpdated: {
            //			Qt.point(pinch.startCenter.x, pinch.startCenter.y)
            //            log.text += "PinchArea onPinchUpdated======= " + pinch.scale + "\n"
            var zoomFactor = (pinch.scale - pinch.previousScale) / pinch.previousScale
            airplaneDrawer.zoomBy(zoomFactor, pinch.startCenter.x, pinch.startCenter.y)
        }
    } // PinchArea

    MouseArea {
        anchors.fill: parent

        property real ix
        property real iy

        property date pressTime
        property var heldTimeMsec
        property real pressX
        property real pressY
        property real moveDistance

        enabled: !pa.isDragging
        onCanceled: {

            //            log.text += "MouseArea onCanceled" + "\n"
        }

        onPositionChanged: {
            //            log.text += "MouseArea onPositionChanged" + "\n"
            if (Math.abs(ix - mouseX) > 1.0 || Math.abs(iy - mouseY) > 1.0) {

                airplaneDrawer.moveBy(ix - mouseX, iy - mouseY)
                //airplaneDrawer.position = Qt.point(airplaneDrawer.position.x - (ix - mouseX), airplaneDrawer.position.y - (iy - mouseY))
                ix = mouseX
                iy = mouseY
            }
        }

        onPressed: {
            //            log.text += "MouseArea onPressed" + "\n"
            ix = mouseX
            iy = mouseY

            pressTime = new Date()
            pressX = mouseX
            pressY = mouseY
        }

        onReleased: {
            heldTimeMsec = new Date().getTime() - pressTime.getTime()
            //            console.log("onReleased:   " + mouseX + " x " + mouseY)
            moveDistance = Math.sqrt(Math.pow((mouseX - pressX), 2) + Math.pow((mouseY - pressY), 2))
        }

        onClicked: {
            if (heldTimeMsec > 600 || moveDistance > 10) {
                //console.log("held:   " + heldTimeMsec + "  dist:" + moveDistance)
                return
            }
            var id = airplaneDrawer.getIdAt(mouseX, mouseY)
        }

        onWheel: {
            var valZoom = wheel.angleDelta.y / 120.0 / 10.0
            airplaneDrawer.zoomBy(valZoom, wheel.x, wheel.y)
        }
    } // MouseArea
    */
    PinchArea {
        id: pa
        anchors.fill: parent
        property bool isDragging: false
        onPinchFinished: pa.isDragging = false
        onPinchStarted: pa.isDragging = true

        onPinchUpdated: {
            var zoomFactor = (pinch.scale - pinch.previousScale) / pinch.previousScale
            airplaneDrawer.zoomBy(zoomFactor, pinch.startCenter.x, pinch.startCenter.y)
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

            enabled: !pa.isDragging

            onPositionChanged: {
                if (Math.abs(ix - mouseX) > 1.0 || Math.abs(iy - mouseY) > 1.0) {
                    airplaneDrawer.moveBy(ix - mouseX, iy - mouseY)
                    ix = mouseX
                    iy = mouseY

                    //                    airplaneControl.positionChanged(airplaneDrawer.position)
                }
            }

            onPressed: {
                ix = mouseX
                iy = mouseY

                pressTime = new Date()
                pressX = mouseX
                pressY = mouseY
            }

            onReleased: {
                heldTimeMsec = new Date().getTime() - pressTime.getTime()
                //                console.log("onReleased:   " + mouseX + " x " + mouseY)
                moveDistance = Math.sqrt(Math.pow((mouseX - pressX), 2) + Math.pow((mouseY - pressY), 2))
            }

            onClicked: {
                if (heldTimeMsec > 600 || moveDistance > 10) {
                    //console.log("held:   " + heldTimeMsec + "  dist:" + moveDistance)
                    return
                }
                var id = airplaneDrawer.getIdAt(mouseX, mouseY)
                if (id !== "")
                    airplaneControl.itemClicked(id)
            }

            onWheel: {
                var valZoom = wheel.angleDelta.y / 120.0 / 10.0
                airplaneDrawer.zoomBy(valZoom, wheel.x, wheel.y)
            }
        } // MouseArea ( inside PinchArea)
    } // PinchArea
}
