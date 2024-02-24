import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import AppQtTest12 1.0

ListView {
    id: jsFlightsJsList

    property var flightsModel

    signal selectedFlight(var flight)

    //signal selectedFlight(Flight flight)

    //            anchors.fill: parent
    //            anchors.topMargin: 85
    // Layout.fillWidth: true
    //  Layout.preferredWidth: 10

    //anchors.bottomMargin: 40
    interactive: contentHeight > height
    clip: true

    //     model: flightsListModel.jsFligths
    //    model: flightsListModel.fligths
    model: flightsModel

    delegate: Item {
        width: jsFlightsJsList.width
        height: 30
        //height: header.height
        opacity: !mouseArea.pressed ? 1 : 0.3 // pressed state
        property int rowIndex: index // outer index

        //property Flight flight: modelData
        property var flight: modelData

        RowLayout {
            spacing: 10
            width: jsFlightsJsList.width
            Layout.fillWidth: true

            // width: jsFlightsJsList.width
            Label {
                Layout.preferredWidth: 10
                text: flight.id
            }

            Label {
                Layout.preferredWidth: 10
                text: flight.number
            }

            Label {
                Layout.preferredWidth: 30
                text: flight.date
            }

            Label {
                Layout.preferredWidth: 10
                //  Layout.fillHeight: true
                //width: jsFlightsJsList.width * 0.3
                text: flight.originAirportCode
            }

            Label {
                Layout.preferredWidth: 20
                //  Layout.fillHeight: true
                //width: jsFlightsJsList.width * 0.3
                text: flight.originCity
            }
        } //Row

        MouseArea {
            id: mouseArea

            anchors.fill: parent

            onClicked: {
                flight.number = flight.number + 1
                selectedFlight(flight)
            }


            /*
            onDoubleClicked: {
                console.log("person", person.value)
                person.value = person.value + 1
                itemMessageDialog.informativeText = JSON.stringify(jsonModel.model.get(rowIndex))
                itemMessageDialog.open()
            }*/
        }
    }
}
