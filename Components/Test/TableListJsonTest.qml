import QtQuick 2.15
import AppQtTest12 1.0
import QtQuick.Layouts

Item {

    id: root

    JsonListModel {
        id: jsonPersonModel
    }

    // signal clicked(int row, variant person)
    PersonEditDialog {
        id: editPersonDialog
        onFinished: function () {}
    }

    function showEditPersonDialog(person) {
        console.log("show person", person.name, person.age)
        editPersonDialog.editPerson(person)
    }

    function incrementAge(index, person) {
        console.log("inc age", person.name, person.age)
        person.age = person.age + 1

        //listPersons.model.dataChanged(0, 10)
        //listPersons.model.dataChanged(listPersons.model.index(0, index - 1), listPersons.model.index(index - 1, 3 - 1))
        //  editPersonDialog.editPerson(person)
    }

    //onClicked: print('onClicked', row, JSON.stringify(rowData))
    GridLayout {
        id: grid
        anchors.fill: parent
        rows: 3
        columns: 1

        //===== PAGE TITLE ====
        Rectangle {
            //id: title
            Layout.row: 1

            color: "gray"
            implicitHeight: txtTitle.height
            border {
                color: "black"
                width: 1
            }

            anchors.left: parent.left
            anchors.right: parent.right
            Text {
                x: 100
                anchors.leftMargin: 50
                font.pointSize: 24
                id: txtTitle
                //anchors.horizontalCenter: parent
                text: "JSON TABLE Person list"
            }
        }

        //===== TABLE HEADER ====
        Rectangle {
            id: header
            Layout.row: 2

            // Layout.fillHeight: true
            Layout.fillWidth: true
            // anchors.topMargin: 200
            //width: parent.width
            height: 30
            color: 'black'

            property variant headerModel: [{
                    "text": 'seat',
                    "width": 0.2
                }, {
                    "text": 'name',
                    "width": 0.5
                }, {
                    "text": 'age',
                    "width": 0.2
                }]

            ListView {

                anchors.fill: parent
                orientation: ListView.Horizontal
                interactive: false

                model: header.headerModel

                delegate: Item {
                    // cell
                    width: modelData.width * root.width
                    height: header.height

                    Text {
                        x: 0.03 * root.width
                        text: modelData.text
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        //font.pixelSize: 0.06 * root.width
                        color: 'white'
                    }
                }
            }
        }

        // ==== DATA ====
        ListView {
            id: listPersons
            Layout.row: 3
            Layout.fillHeight: true
            Layout.fillWidth: true

            interactive: contentHeight > height
            clip: true

            // model: dataModel
            model: jsonPersonModel.persons

            delegate: Item {
                // row
                width: root.width
                height: 30
                //height: header.height
                opacity: !mouseArea.pressed ? 1 : 0.3 // pressed state

                property int rowIndex: index // outer index
                property variant person: modelData // much faster than listView.model[rowIndex]

                Row {
                    anchors.fill: parent

                    //height: header.height

                    //height: 100
                    // index is column
                    //model: rowData // headerModel.length
                    Text {
                        height: header.height
                        width: header.headerModel[0].width * root.width
                        //height: header.height
                        text: person.seat
                    }

                    Text {
                        width: header.headerModel[1].width * root.width
                        //height: header.height
                        text: person.name
                    }

                    Text {
                        width: header.headerModel[2].width * root.width
                        //height: header.height
                        text: person.age
                        //font.pixelSize: 0.06 * root.width
                    }
                }

                MouseArea {
                    id: mouseArea

                    anchors.fill: parent

                    //onClicked: root.clicked(rowIndex, person)
                    onClicked: incrementAge(rowIndex, person)
                    onDoubleClicked: showEditPersonDialog(person)
                }
            }
            //ScrollBar {}
        } // list table DATA
    }
}
