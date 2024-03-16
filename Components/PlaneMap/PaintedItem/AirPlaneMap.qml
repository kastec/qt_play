//import QtQuick 2.0
//import Sailfish.Silica 1.0
//import QtQuick.Layouts 1.1
//import CrewTablet 1.0
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import AppQtTest12 1.0

Item {

    id: airplaneMapControl

    //    signal itemClicked(var id)
    AirplaneViewModel {
        id: airplaneViewModel
    }

    Component.onCompleted: {
        loadLayout()
    }

    function ensureVisible(seatName) {
        if (seatName === "")
            return
        //Logger.logInfo("scroll plane map to " + seatName)
        airplaneViewControl.scrollTo(seatName)
    }

    function loadLayout() {
        airplaneViewModel.loadLayout()
    }

    RowLayout {
        anchors.fill: parent
        spacing: 20

        AirPlaneViewControl {
            id: airplaneViewControl
            viewModel: airplaneViewModel

            //            planeMap: airplaneViewModel.planeMap
            onItemClicked: function (id) {
                console.log("clicked seat on: " + id)
            }

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: 10
            //            onPositionChanged: function (pos) {
            //                airplaneNavControl.setMapPosition(pos)
            //            }
        }

        AirPlaneNavControl {
            id: airplaneNavControl
            viewModel: airplaneViewModel
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: 2
            //            width: 150
        }
    }
    Button {
        anchors.right: parent.right
        text: "RELOAD"
        onClicked: loadLayout()
    }
}
