import QtQuick 2.15
import QtQuick.Controls
import AppQtTest12 1.0
import QtQuick.Layouts

Item {
    //    anchors.fill: parent
    ColumnLayout {
        anchors.fill: parent
        Image {
            source: "image://colors/yellow"
            width: 300
            height: 50
        }
        Image {
            source: "image://colors/red"
            width: 100
            height: 50
        }

        Button {
            text: "reload"
            onClicked: liveImage.reload()
        }

        Image {
            id: liveImage
            property bool counter: false

            //            Layout.fillHeight: true
            //            Layout.preferredWidth: parent.width / 2
            //            Layout.preferredHeight: parent.height / 2
            width: 300
            height: 300

            asynchronous: true
            source: "image://live/image?id=" + counter
            //            source: "file:///C:/Projects/QT_Test/QtTest1/Components/PlaneMap/loadedCrew.jpg"
            //        anchors.fill: parent
            //                        fillMode: Image.PreserveAspectFit
            fillMode: Image.PreserveAspectCrop
            cache: false

            sourceSize.width: parent.width / 2
            sourceSize.height: parent.height / 2
            function reload() {
                counter = !counter
                source = "image://live/image?id=" + counter
            }
        }
    }
}
