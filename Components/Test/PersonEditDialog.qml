import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: dialog

    signal finished

    title: "Edit Person dialog"
    anchors.centerIn: parent
    standardButtons: Dialog.Ok //| Dialog.Cancel

    implicitWidth: parent.width / 2
    implicitHeight: parent.height / 2

    focus: true
    modal: true

    property variant personModel
    contentItem: GridLayout {
        id: grid

        rows: 1
        columns: 2

        Label {
            text: "Full Name"
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
        }

        Label {
            text: personModel.name
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
        }

        Label {
            text: "Age"
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
        }

        Label {
            text: personModel.age
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
        }


        /*
        Label {
            text: qsTr("Address")
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
        }

        TextField {
            id: address
            Layout.fillWidth: true
            Layout.minimumWidth: grid.minimumInputSize
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            placeholderText: grid.placeholderText
        }

        Label {
            text: qsTr("City")
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
        }

        TextField {
            id: city
            Layout.fillWidth: true
            Layout.minimumWidth: grid.minimumInputSize
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            placeholderText: grid.placeholderText
        }

        Label {
            text: qsTr("Number")
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
        }

        TextField {
            id: number
            Layout.fillWidth: true
            Layout.minimumWidth: grid.minimumInputSize
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            placeholderText: grid.placeholderText
        }
        */
    }

    function editPerson(person) {
        personModel = person
        dialog.open()
    }
}
