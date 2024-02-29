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

    signal itemClicked(var id)

    AirplaneViewModel {
        id: airplaneViewModel
    }

    Component.onCompleted: {
        airplaneViewModel.loadLayout()
    }

    function ensureVisible(seatName) {
        if (seatName === "")
            return
        //Logger.logInfo("scroll plane map to " + seatName)
        airplaneViewControl.scrollTo(seatName)
    }

    RowLayout {
        anchors.fill: parent
        spacing: 20

        AirPlaneViewControl {
            id: airplaneViewControl
			viewModel: airplaneViewModel
            planeMap: airplaneViewModel.planeMap

            onItemClicked: airplaneMapControl.itemClicked(id)

            Layout.fillHeight: true
            Layout.fillWidth: true

            onPositionChanged: {
                //                console.log(pos)
                airplaneNavControl.setMapPosition(pos)
            }
        }

        AirPlaneNavControl {
            id: airplaneNavControl
            planeMap: airplaneViewModel.planeMap
            Layout.fillHeight: true
            width: 190
        }
    }
}
