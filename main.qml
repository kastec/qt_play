import QtQuick
import QtQuick.Controls
import AppQtTest12 1.0
import QtQuick.Layouts
import "./Components/"
import "./Components/PersonList"
import "./Components/JsonList"
import "./Components/MainLayout"
import "./Components/PlaneMap"
import "./Components/PlaneMap/PaintedItem"
import "./Components/PlaneMap/OpGlItem"

Window {
    width: 1200
    height: 700
    visible: true
    title: qsTr("Hello World")

    SwipeView {
        id: swipeView
        anchors.fill: parent

        AirPlaneMap {}
        //        AirPlaneViewControl {}
        //2 FlickPlan {}
        //1        PlaneMap {}

        //        LayoutCtrl {}
        //        JsonListComp {}
        //        PersonListCtrl {}
        //        BookListCtrl {}
    }
    PageIndicator {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        currentIndex: swipeView.currentIndex
        count: swipeView.count
    }


    /*
    BookListCtrl {
        id: bookListComp
        anchors.fill: parent
    }

*/


    /*   PersonListCtrl {
        anchors.fill: parent
    }
    */
}
