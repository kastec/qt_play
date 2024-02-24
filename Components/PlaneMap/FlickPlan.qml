import QtQuick 2.15
import QtQuick.Controls
import AppQtTest12 1.0
import QtQuick.Layouts

Item {

    id: mainId
    width: 1280
    height: 720
    property int scaleMultiplier: 3
    property real zoom

    onZoomChanged: {
        var zoomPoint = Qt.point(flickArea.width / 2 + flickArea.contentX, flickArea.height / 2 + flickArea.contentY)

        flickArea.resizeContent((inner.width * zoom), (inner.height * zoom), zoomPoint)
        flickArea.returnToBounds()
    }

    Flickable {
        id: flickArea
        anchors.fill: parent
        focus: true
        anchors.centerIn: parent
        boundsBehavior: Flickable.StopAtBounds

        contentWidth: Math.max(inner.width * slider.value * scaleMultiplier, width)
        contentHeight: Math.max(inner.height * slider.value * scaleMultiplier, height)
        contentX: contentWidth == width ? 0 : inner.width * slider.value * scaleMultiplier / 2 - flickArea.width / 2
        contentY: contentHeight == height ? 0 : inner.height * slider.value * scaleMultiplier / 2 - flickArea.height / 2

        //        contentWidth: inner.width
        //        contentHeight: inner.height
        Image {
            id: inner
            property bool counter: false
            //scale: slider.value * scaleMultiplier
            scale: zoom
            anchors.centerIn: parent

            //            source: "file:///C:/Projects/QT_Test/QtTest1/Components/PlaneMap/loadedCrew.jpg"
            source: "image://live/image?id=" + counter

            //            source: "image://live/image?id="

            //            transform: Scale {
            //                id: scaleID
            //                origin.x: flickArea.contentX + flickArea.width * flickArea.visibleArea.widthRatio / 2
            //                origin.y: flickArea.contentY + flickArea.height * flickArea.visibleArea.heightRatio / 2
            //            }
            sourceSize.width: flickArea.contentWidth
            sourceSize.height: flickArea.contentHeight
            function reload() {
                counter = !counter
                source = "image://live/image?id=" + counter
            }
        }
        //        Rectangle {
        //            id: inner
        //            width: 500
        //            height: 500
        //            color: "red"
        //            scale: zoom
        //            anchors.centerIn: parent
        //        }
    }
    Label {
        anchors.top: parent.top
        anchors.topMargin: 100
        text: flickArea.contentX + "," + flickArea.contentY + "[" + flickArea.contentWidth + "x" + flickArea.contentHeight + "]"
    }

    Slider {
        id: slider
        value: .01
        orientation: Qt.Vertical
        anchors {
            bottom: parent.bottom
            right: parent.right
            top: parent.top
            margins: 50
        }

        from: 0.01

        onValueChanged: {
            zoom = value * scaleMultiplier
        }
    }
    Button {
        text: "reload"
        onClicked: inner.reload()
    }
}
