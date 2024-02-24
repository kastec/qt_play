//import QtQuick 2.0
//import Sailfish.Silica 1.0
//import QtQuick.Layouts 1.1
//import CrewTablet 1.0
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import AppQtTest12 1.0

Item {

	property alias planeMap: airplaneNavDrawer.planeMap
	
	
	function setMapPosition(pos)
	{
		airplaneNavDrawer.setMapPosition(pos)
	}
	
    Rectangle {
        color: "#D9E6ED"
        anchors.fill: parent
        radius: 32
		
		AirplaneNavDrawer {
			id: airplaneNavDrawer
			planeMap: airplaneViewModel.planeMap
	
			anchors.fill: parent
	
			
		} // AirplaneDrawer
		
    }
}
