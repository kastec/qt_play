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
import "./Components/Catering"

Window {
    width: 1200
    height: 800
    visible: true
    title: qsTr("Hello World")

    CaterPlane {}

    /*
    SwipeView {
        id: swipeView

        anchors.fill: parent

        //		plane render
        // AirPlaneMap {}
        //CaterPlane {}
        //2 FlickPlan {}
        //1        PlaneMap {}
        //        LayoutCtrl {}
        //JsonListComp {}
        //InternetConnectinTest {}
        //        BookListCtrl {}
        //         PersonListCtrl {}
    }
    PageIndicator {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        currentIndex: swipeView.currentIndex
        count: swipeView.count
    }
*/


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
