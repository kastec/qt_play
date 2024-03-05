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

    //    property alias planeMap: airplaneDrawer.planeMap
    property size dimensions: Qt.size(width, height)

    signal itemClicked(var id)

    Component.onCompleted: {
        //        airplaneDrawer.changeVisibleSize(dimensions)
        viewModel.addPainter("airplane", planeMapArea)
    }
    function scrollTo(scrollToId) {
        var vm = rootControl.viewModel

        var z = vm.zoom

        //        if (z < 1.1) z = 1.1
        //        if (z > 2.2) z = 2.2
        //        animateZoom.from = vm.zoom
        //        animateZoom.to = z
        var pos = vm.getMoveToCenterAt(scrollToId, z)
        animatePos.from = viewModel.position
        animatePos.to = pos

        animateZoom.start()
        animatePos.start()
    }

    PropertyAnimation {
        id: animateZoom
        target: rootControl.viewModel
        properties: "zoom"
        from: 1.0
        to: 2.5
        duration: 300
    }
    PropertyAnimation {
        id: animatePos
        target: rootControl.viewModel
        properties: "position"
        from: Qt.point(0, 0)
        to: Qt.point(500, 500)
        duration: 300
    }

    Column {

        z: 100

        Button {
            text: viewModel.position.x
            onClicked: {
                animatePos.from = rootControl.viewModel.position
                animatePos.start()
            }
        }

        Button {
            text: "zoom:" + rootControl.viewModel.zoom
            onClicked: {
                animateZoom.start()
            }
        }

        Button {
            text: "4C"
            onClicked: rootControl.scrollTo(text)
        }
        Button {
            text: "18F"
            onClicked: rootControl.scrollTo(text)
        }
        Button {
            text: "50B"
            onClicked: rootControl.scrollTo(text)
        }

        Button {
            text: "WC6"
            onClicked: rootControl.scrollTo(text)
        }

        Button {
            text: viewModel.avgRenderTime + " / " + viewModel.renderTime
            //            onClicked: log.text = ""
        }

        Button {
            text: "move 0,0"
            //            onClicked: airplaneDrawer.position = Qt.point(0, 0)
        }
        Button {
            text: "move 300,300"
            //            onClicked: airplaneDrawer.position = Qt.point(300, 300)
        }

        Button {
            text: "zoom 0.5"
            onClicked: rootControl.viewModel.zoom = 0.5
        }
        Button {
            text: "zoom 1"
            onClicked: rootControl.viewModel.zoom = 1.0
        }
        Button {
            text: "zoom 2"
            onClicked: rootControl.viewModel.zoom = 2.0
        }
    }

    onDimensionsChanged: {
        //        airplaneDrawer.changeVisibleSize(dimensions)
        rootControl.viewModel.changeVisibleSize(dimensions)
    }

    PaintArea {
        id: planeMapArea
        anchors.fill: parent
    }

    PinchArea {
        id: pa
        anchors.fill: parent
        property bool isDragging: false
        onPinchFinished: pa.isDragging = false
        onPinchStarted: pa.isDragging = true

        onPinchUpdated: {
            var zoomFactor = (pinch.scale - pinch.previousScale) / pinch.previousScale
            //            airplaneDrawer.zoomBy(zoomFactor, pinch.startCenter.x, pinch.startCenter.y)
            //2nd
            rootControl.viewModel.zoomBy(zoomFactor, pinch.startCenter.x, pinch.startCenter.y)
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
                    //                    airplaneDrawer.moveBy(ix - mouseX, iy - mouseY)
                    rootControl.viewModel.moveBy(ix - mouseX, iy - mouseY)
                    ix = mouseX
                    iy = mouseY

                    // airplaneControl.positionChanged(airplaneDrawer.position)
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

                //                var id = airplaneDrawer.getIdAt(mouseX, mouseY)
                //                if (id !== "")
                //                    rootControl.itemClicked(id)
                var idSeat = rootControl.viewModel.getIdAt(mouseX, mouseY)
                if (idSeat !== "")
                    rootControl.itemClicked(idSeat)
            }

            onWheel: {
                var valZoom = wheel.angleDelta.y / 120.0 / 10.0
                //                airplaneDrawer.zoomBy(valZoom, wheel.x, wheel.y)
                rootControl.viewModel.zoomBy(valZoom, wheel.x, wheel.y)
            }
        } // MouseArea ( inside PinchArea)
    } // PinchArea
}
