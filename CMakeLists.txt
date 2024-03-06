cmake_minimum_required(VERSION 3.16)

project(QtTest1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Network REQUIRED)


qt_add_executable(appQtTest1
    main.cpp
)

qt_add_qml_module(appQtTest1
    URI QtTest1
    VERSION 1.0   
    QML_FILES
        main.qml ElemTest.qml
        Components/BookListCtrl.qml
        Components/PersonList/PersonListCtrl.qml
        Components/PersonList/PersonEditCtrl.qml
        
        Components/PlaneMap/PlaneMap.qml
        Components/PlaneMap/FlickPlan.qml
        Components/PlaneMap/PaintedItem/AirPlaneViewControl.qml
        Components/PlaneMap/PaintedItem/AirPlaneNavControl.qml
        Components/PlaneMap/PaintedItem/AirPlaneMap.qml  
                
        Components/PlaneMap/OpGlItem/OGLItem.qml
                
        Components/JsonList/JsonListComp.qml        

        Components/MainLayout/LayoutCtrl.qml

        Components/Test/TableListJsonTest.qml
        Components/Test/PersonEditDialog.qml

        Components/Flights/FlightsList.qml
        Components/Flights/FlightListControl.qml
        
        Utils/QmlTools/JSONListModel/JSONListModel.qml
        Utils/QmlTools/JSONListModel/jsonpath.js 
        
      
    SOURCES
        Utils/nameoff.h
        Utils/JsonHelper/JsonHelper.h
        Utils/JsonHelper/FileHelper.h
        Utils/JsonHelper/qqmlhelper.h
        Utils/JsonHelper/JsonSerialization.h
        Components/Test/testAsyncCall.h
        
        Components/Flights/flight.h 
        
        Classes/bookitem.h Classes/bookitem.cpp
        Classes/booklist.h Classes/booklist.cpp
        Utils/HttpRequester/httpRequester.h Utils/HttpRequester/httpRequester.cpp
 
        Components/PersonList/person.h Components/PersonList/person.cpp
        Components/PersonList/personlistmodel.h Components/PersonList/personlistmodel.cpp
        Components/Flights/flightslistmodel.h Components/Flights/flightslistmodel.cpp

        Utils/datasamples.h Utils/datasamples.cpp
        Components/Test/jsonlistmodel.h Components/Test/jsonlistmodel.cpp
        
        Components/PlaneMap/liveimageprovider.h Components/PlaneMap/liveimageprovider.cpp
        Components/PlaneMap/colorImageProvider.h        
        Components/PlaneMap/OpGlItem/glitem.h Components/PlaneMap/OpGlItem/glitem.cpp
        
        Components/PlaneMap/PaintedItem/paintArea.h
       Components/PlaneMap/PaintedItem/src/planeItemBase.h Components/PlaneMap/PaintedItem/src/planeItemBase.cpp
        Components/PlaneMap/PaintedItem/src/planeMap.h Components/PlaneMap/PaintedItem/src/planeMap.cpp
        Components/PlaneMap/PaintedItem/src/planeItemChair.h Components/PlaneMap/PaintedItem/src/planeItemChair.cpp
        Components/PlaneMap/PaintedItem/src/planeItemExit.h Components/PlaneMap/PaintedItem/src/planeItemExit.cpp
        Components/PlaneMap/PaintedItem/src/planeColors.h
        Components/PlaneMap/PaintedItem/src/planeLayoutParser.h
        Components/PlaneMap/PaintedItem/src/spriteCache.h 
        Components/PlaneMap/PaintedItem/src/frameRateCalc.h
        Components/PlaneMap/PaintedItem/src/planeLayoutSearcher.h
        Components/PlaneMap/PaintedItem/airplaneViewModel.h Components/PlaneMap/PaintedItem/airplaneViewModel.cpp
        Components/PlaneMap/PaintedItem/src/renderBuffer.h
        Components/PlaneMap/PaintedItem/src/planePassengers.h
        Components/PlaneMap/PaintedItem/src/cardServiceType.h
        Components/PlaneMap/PaintedItem/src/planeNavDrawer.h 
         
        
        Utils/JsonHelper/serializationBinder.h
        Components/Test/derivedDeserializeTest.h
    )

set_target_properties(appQtTest1 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appQtTest1  
    PRIVATE Qt6::Quick)

target_link_libraries(appQtTest1  
    PRIVATE Qt6::Network)

install(TARGETS appQtTest1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


