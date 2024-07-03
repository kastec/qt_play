import QtQuick 2.15
import AppQtTest12 1.0

Item {
    anchors.fill: parent

    property var caterPlaneViewModel: CaterPlaneViewModel {}

    Rectangle {
        width: parent.width * 0.7
        height: parent.height

        anchors.topMargin: 30
        anchors.leftMargin: 30

        border.color: "#D1D1D1"
        border.width: 2
        Flickable {
            id: flickArea
            anchors.fill: parent

            clip: true

            interactive: true
            boundsBehavior: Flickable.StopAtBounds
            contentWidth: 2500
            contentHeight: 2500
            flickableDirection: Flickable.VerticalFlick

            onContentYChanged: tbInfo.text = contentY
            //            bottomMargin: 100
            //            leftMargin: 150
            //            rightMargin: 150
            //            topMargin: 200

            //        rebound: Transition {
            //            NumberAnimation {
            //                properties: "x,y"
            //                duration: 1000
            //                easing.type: Easing.OutBounce
            //            }
            //        }

            //		contentWidth: Math.max(inner.width * slider.value * scaleMultiplier, width)
            //        contentHeight: Math.max(inner.height * slider.value * scaleMultiplier, height)
            Rectangle {
                x: 1200
                y: 1700
                width: 300
                height: 300
                color: "green"
            }

            Repeater {
                model: caterPlaneViewModel.itemsDisplay

                CaterSeat {
                    x: modelData.x
                    y: modelData.y
                    caterSeat: modelData
                    // MouseArea {
                    //    anchors.fill: parent
                    //    drag.target: parent
                    // }
                }
            } // REPEATER
        } // FLICABLE

        Text {
            id: tbInfo
        }
    }
}
