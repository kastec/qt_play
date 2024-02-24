import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import "../PersonList"
import "../Test"
import "../Flights"

Item {
    id: root
    Column {

        anchors.fill: parent

        StackLayout {

            id: layout

            anchors.top: parent.top
            anchors.bottom: buttonNavs.top
            anchors.left: parent.left
            anchors.right: parent.right

            currentIndex: 0

            FlightsList {}

            TableListJsonTest {}
            PersonListCtrl {}

            Rectangle {
                color: "lightblue"
                radius: 10.0
                width: 300
                height: 50
                Text {
                    anchors.centerIn: parent
                    font.pointSize: 24
                    text: "Books"
                }
            }
            Rectangle {
                color: "gold"
                radius: 10.0
                width: 300
                height: 50
                Text {
                    anchors.centerIn: parent
                    font.pointSize: 24
                    text: "Music"
                }
            }
            Rectangle {
                color: "lightgreen"
                radius: 10.0
                width: 300
                height: 50
                Text {
                    anchors.centerIn: parent
                    font.pointSize: 24
                    text: "Movies"
                }
            }
        }

        Row {
            id: buttonNavs

            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: Math.max(root.height / 8, 30)

            spacing: 20

            Button {
                text: "First"
                onClicked: layout.currentIndex = 0
            }

            Button {
                text: "Second"
                onClicked: layout.currentIndex = 1
            }

            Button {
                text: "Third"
                onClicked: layout.currentIndex = 2
            }
        }
    }
}
