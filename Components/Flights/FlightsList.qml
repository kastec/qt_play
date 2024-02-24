import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import AppQtTest12 1.0

import "../../Utils/QmlTools/JSONListModel/"

Item {
    anchors.fill: parent

    //property Flight cF
    property var cF

    FlightsListModel {
        id: flightsListModel
    }
    ColumnLayout {
        id: colControl
        anchors.fill: parent

        Text {

            text: "Load Http data"
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
        }

        Text {
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            //			text: "Loaded: %1".arg(flightsListModel.jsFligths !== null ? flightsListModel.jsFligths.length : 0)
            text: "Loaded: %1".arg(flightsListModel.fligths !== null ? flightsListModel.fligths.length : 0)
        }

        Row {
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            Button {
                //                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                text: "Load Flights-1"
                onClicked: {
                    flightsListModel.loadJsonArray()
                }
            }
            Item {
                width: 10
                height: 1
            }
            Button {
                //                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                text: "Load Flights-2"
                onClicked: {
                    flightsListModel.loadJsonArray2()
                }
            }
            Item {
                width: 10
                height: 1
            }
            Button {
                //                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                text: "Save Flights"
                onClicked: {
                    flightsListModel.saveFlights()
                }
            }
        }
        TextInput {
            id: editFlight
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            //anchors.top: btLoad.bottom
            //anchors.bottom: flightLitsContainer.top
            height: 15

            text: cF.originCity
            // text: (flightsListModel.curFlight != null) ? flightsListModel.curFlight.number : ""

            //text: flightsListModel.qvarFlight.originCity
            //            text: flightsListModel.jscurFlight.originCity

            // text: "Number here: %1".arg(cF.originCity)

            //text: flightsListModel.qvarFlight.originCity
            //text: "Number here: %1".arg(flightsListModel.curFlight)
            //text: "Number here: %1".arg(flightsListModel.qvarFlight.number)
            //text: "Number here: %1".arg(flightsListModel.jscurFlight.number)

            //text: "Number here: %1".arg(flightsListModel.jscurFlight.number)
            //  text: "Number here: %1".arg(cF.number)

            //text: flightsListModel.curFlight.number
            //			text: flightsListModel.curFlight !== null ? lightsListModel.curFlight.number : 0
        }
        Button {
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            id: btUpd
            text: "Update value"
            onClicked: {
                cF.originCity = editFlight.text
                //flightsListModel.qvarFlight.originCity = editFlight.text
                //                flightsListModel.jscurFlight.originCity = editFlight.text
            }
        }
        RowLayout {
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            id: flightLitsContainer
            width: parent.width

            Layout.fillHeight: true
            Layout.fillWidth: true

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredWidth: 100

                Rectangle {
                    anchors.fill: parent
                    color: "red"
                    opacity: 0.3
                }

                Text {
                    text: "JSON here"
                    y: -15
                }

                FlightListControl {
                    anchors.fill: parent
                    flightsModel: flightsListModel.jsFligths
                    onSelectedFlight: flight => {
                                          console.log("Json flight:", flight)
                                          flightsListModel.qvarFlight = flight
                                          cF = flight

                                          // console.log("Json2 flight:", flightsListModel.jscurFlight)
                                          //  flightsListModel.currentFlight = flight
                                      }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredWidth: 100

                Rectangle {
                    anchors.fill: parent
                    color: "green"
                    opacity: 0.3
                }

                Text {
                    text: "QList<FLIGHT> here"
                    y: -15
                }

                FlightListControl {
                    anchors.fill: parent
                    flightsModel: flightsListModel.fligths

                    onSelectedFlight: flight => {
                                          console.log("obj flight:", flight)

                                          flightsListModel.curFlight = flight
                                          flightsListModel.qvarFlight = flight
                                          cF = flight
                                          console.log("obj2 flight:", flightsListModel.curFlight.number)

                                          flightsListModel.curFlight.info()

                                          //editFlight.text = flightsListModel.curFlight.number

                                          // editFlight.text = flightsListModel.curFlight.number
                                          //                                          flightsListModel.currentFlight = flight
                                      }
                }
            }
        }
    }
}
