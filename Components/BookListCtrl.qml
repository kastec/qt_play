import QtQuick 2.15
import QtQuick.Controls
import AppQtTest12 1.0

Item {
    id: root

    BookList {
        id: dataModel
    }

    Column {

        anchors.fill: parent

        Rectangle {

            id: rowControls
            anchors.top: parent.top

            anchors.left: parent.left
            anchors.right: parent.right
            color: "lightgreen"

            height: 50

            Row {

                spacing: 20
                Item {
                    height: 1
                    width: 50
                }

                anchors.verticalCenter: parent.verticalCenter

                Button {

                    //anchors.leftMargin: 50
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Add Item"
                    onClicked: dataModel.add()
                }

                Button {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Replace List"
                    onClicked: {
                        AppMessageBus.logEvent("EVENT Relace list")
                        dataModel.updateListData()
                    }
                }

                Text {

                    anchors.verticalCenter: parent.verticalCenter
                    renderType: Text.NativeRendering
                    text: "len: %1".arg(dataModel.data.length)
                }
            }
        }

        ListView {

            id: view

            anchors.top: rowControls.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            anchors.margins: 10

            spacing: 10
            model: dataModel.data
            clip: true

            highlight: Rectangle {
                color: "skyblue"
            }
            highlightFollowsCurrentItem: true

            delegate: Item {
                id: listDelegate

                property var view: ListView.view
                property var isCurrent: ListView.isCurrentItem

                //property BookItem bookitem: dataModel.getBookItem(index)
                width: view.width
                height: 40

                // property var dd: bookitem.color
                Rectangle {
                    anchors.margins: 5
                    anchors.fill: parent
                    radius: height / 2
                    color: model.color

                    //color: bookitem.color
                    border {
                        color: "black"
                        width: 1
                    }

                    Text {

                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        //text: bookitem.text
                        text: "%1%2".arg(model.text).arg(isCurrent ? " *" : "")
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: view.currentIndex = model.index
                    }
                }
            }
        }
    }
}
