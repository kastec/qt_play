import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

//import "Utils/QmlTools/JSONListModel/"
import "../../Utils/QmlTools/JSONListModel/"

// https://wiki.qt.io/JSONListModel
//https://github.com/kromain/qml-utils/
Column {
    id: root

    width: 400 //parent.width
    height: 400 //parent.height

    JSONListModel {
        id: jsonModel

        //source: root.jsonData

        //json: root.jsonData
        json: '[ \
{"title": "Answer", "value": 42}, \
{"title": "Pastis", "value": 51}, \
{"title": "Alsace", "value": 67}
]'

        // Labels starting with 'A'
        //query: "$[?(@.title.charAt(0)==='A')]"
    }

    Column {
        spacing: 5
        anchors.fill: parent
        anchors.margins: 5
        anchors.bottomMargin: 0

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
                x: 20
                //anchors.leftMargin: 20
                font.pointSize: 24
                id: txtTitle
                //anchors.horizontalCenter: parent
                text: "JSONListModel provider (json updates)"
            }
        }

        Label {
            anchors.top: parent.top
            anchors.topMargin: title.height
            // height: implicitHeight
            text: "single click - increment, double click - view updated json"
        }

        ListView {
            id: listPersons
            anchors.fill: parent
            anchors.topMargin: 85
            anchors.bottomMargin: 40

            ///Layout.row: 3
            ///Layout.fillHeight: true
            ///Layout.fillWidth: true
            interactive: contentHeight > height
            clip: true

            // model: dataModel
            model: jsonModel.model

            delegate: Item {
                width: listPersons.width
                height: 30
                //height: header.height
                opacity: !mouseArea.pressed ? 1 : 0.3 // pressed state
                property int rowIndex: index // outer index
                property variant person: model // much faster than listView.model[rowIndex]

                Row {
                    spacing: 10
                    Label {
                        width: listPersons.width * 0.1
                        Layout.fillHeight: true
                        Layout.preferredWidth: view.width * 0.2
                        text: "123"
                    }

                    Label {
                        width: listPersons.width * 0.4
                        anchors.verticalCenter: parent
                        renderType: Text.NativeRendering
                        //text: model.title
                        text: person.title

                        //text: modelData.title
                        // font.italic: !person.enabled
                        // color: person.enabled ? "black" : "lightgray"
                    }

                    Label {
                        Layout.fillHeight: true

                        Layout.preferredHeight: implicitHeight
                        Layout.preferredWidth: view.width * 0.3

                        //text: person.enabled
                        text: person.value
                    }
                } //Row

                MouseArea {
                    id: mouseArea

                    anchors.fill: parent
                    onClicked: person.value = person.value + 1
                    onDoubleClicked: {
                        console.log("person", person.value)
                        person.value = person.value + 1
                        itemMessageDialog.informativeText = JSON.stringify(jsonModel.model.get(rowIndex))
                        itemMessageDialog.open()
                    }
                }
            }
        }

        Button {
            anchors.top: parent.bottom - implicitHeight
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10

            // anchors.bottomMargin: 10
            text: "View Json"
            //onClicked: messageDialog.showText(getUpdatedJson())
            onClicked: messageDialog.showJson(jsonModel.model)
        }


        /* MessageDialog {
            id: messageDialog
            icon: StandardIcon.Question

            title: "Json content"
            text: "jlkjl" //jsonModel.json
            // standardButtons: StandardButton.Ok
            onAccepted: {

                //console.log("And of course you could only agree.")
                //Qt.quit()
            }
            // Component.onCompleted: visible = true
        }*/
        MessageDialog {
            id: itemMessageDialog
            title: "Json content"
            text: "Json text:"
            //informativeText: JSON.stringify(jsonModel.model)
            buttons: MessageDialog.Ok

            // onAccepted: Qt.quit()
        }
        MessageDialog {
            id: messageDialog
            title: "Json content"
            text: "Updated Json content text:"
            //informativeText: JSON.stringify(jsonModel.model)
            buttons: MessageDialog.Ok

            function getJson(jmodel) {
                const datamodel = []
                for (var i = 0; i < jmodel.count; ++i)
                    datamodel.push(jmodel.get(i))
                var keysList = JSON.stringify(datamodel)
                return keysList
            }

            function showJson(jmodel) {
                informativeText = getJson(jmodel)
                open()
            }
        }
    }
}
