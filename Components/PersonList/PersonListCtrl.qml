import QtQuick 2.15
import QtQuick.Controls
import AppQtTest12 1.0
import QtQuick.Layouts

Item {

    PersonListModel {
        id: personListModel
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: personListCtrl
    }

    PersonEditCtrl {
        id: personEditCtrl
        visible: false


        /*onUpdateEditPerson: mess => {
                                console.log("got signal from child:", mess)
                                stackView.pop()
                            }*/
    }

    Connections {
        target: personEditCtrl
        function onUpdateEditPerson(mess) {
            console.log("got signal from child", mess)
            stackView.pop()
        }
    }

    Column {

        id: personListCtrl

        anchors.fill: parent

        Rectangle {
            id: title
            color: "gray"

            implicitHeight: txtTitle.height
            border {
                color: "black"
                width: 1
            }

            //anchors.leftMargin: 100
            anchors.top: parent.top

            anchors.left: parent.left
            anchors.right: parent.right
            Text {
                x: 100
                anchors.leftMargin: 50
                font.pointSize: 24
                id: txtTitle
                //anchors.horizontalCenter: parent
                text: "Person list"
            }
        }

        ListView {

            id: view

            //anchors.fill: parent
            anchors.top: title.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10
            spacing: 5
            model: personListModel.persons
            clip: true

            highlight: Rectangle {
                color: "skyblue"
            }
            highlightFollowsCurrentItem: true

            delegate: Item {

                id: listDelegate
                property var view: ListView.view
                property var isCurrent: ListView.isCurrentItem
                property Person person: personListModel.getPerson(index)

                width: view.width
                height: 30

                Rectangle {
                    visible: (!isCurrent && !person.enabled)
                    width: view.width
                    height: listDelegate.height
                    color: "DarkGray"
                }

                RowLayout {

                    //person.age: 100
                    width: view.width
                    height: listDelegate.height

                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: view.width * 0.2
                        text: "123"
                    }

                    Label {
                        Layout.fillHeight: true
                        //Layout.fillWidth: true
                        Layout.preferredHeight: implicitHeight
                        Layout.preferredWidth: view.width * 0.5

                        anchors.verticalCenter: parent
                        renderType: Text.NativeRendering
                        text: "%1%2 (age: %3)".arg(person.name).arg(isCurrent ? " *" : "").arg(person.age)
                        font.italic: !person.enabled
                        color: person.enabled ? "black" : "lightgray"
                    }

                    Label {
                        Layout.fillHeight: true

                        Layout.preferredHeight: implicitHeight
                        Layout.preferredWidth: view.width * 0.3

                        //text: person.enabled
                        text: person.seat
                    }
                } // row

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        view.currentIndex = model.index
                        //var pr = personListModel.getPerson(index)
                        personListModel.log(model.index)
                        //pr.age = 100
                        //personListModel.log(model.index)
                    }
                    onDoubleClicked: {
                        var person = personListModel.getPerson(model.index)
                        //stackView.push(personEditCtrl)
                        stackView.push(personEditCtrl, {
                                           "person": person
                                       })
                    }
                }
            } // item
        } // list
    } // Column
}
