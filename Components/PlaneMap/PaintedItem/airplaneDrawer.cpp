#include "QDebug"

#include "airplaneDrawer.h"
#include <QImageReader>
#include <QPainterPath>
#include <QPainter>

#include <QtConcurrent/QtConcurrentRun>

#include <future>
#include <iostream>
#include <thread>
#include <QFutureWatcher>
#include <functional>
#include "./src/planeLayoutParser.h"


AirplaneDrawer::AirplaneDrawer(QQuickItem *parent) : QQuickPaintedItem(parent), position(QPoint(0,0)), zoom(1.0)
{  
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    
 //TODO: load planeLayoutDescription, pass it to createLayout
//    planeMap = new PlaneMap();
//    planeMap->createLayout();
}


    
void AirplaneDrawer::changeVisibleSize(QSize size){
    screenSize = size;
    
//    if(planeMap!=nullptr)
//        planeMap->setScreenSize(screenSize);
        
    auto screenWidth = screenSize.width();

    auto chairWidth = PlaneLayoutParser::CHAIR_SIZE;
    maxZoom = screenWidth / (qreal)(3*chairWidth); // кол-во кресел * 100
    
    auto planeWidth = (12 *chairWidth); // ширина борта в пикселях (кол-во кресел * 100)
    minZoom = screenWidth / (qreal)(4*planeWidth); // 2 ширины борта  убирается на экране
}

// --- Props
bool AirplaneDrawer::onPositionChanging(QPoint newPos){
    position = newPos;
  //  qDebug() <<"pos:"<< newPos ;
    update();
    return true;
}

bool AirplaneDrawer::onZoomChanging(qreal newZoom){        
    auto zoomFactor = (newZoom-zoom)/zoom;
    bool res = zoomBy(zoomFactor,screenSize.width()/2, screenSize.height()/2);
    return res; 
}

//---------

void AirplaneDrawer::paint(QPainter *painter)
{
     auto xpos = position.x();
     auto ypos = position.y();

     framerate.start();
     
     // PLANE RENDER
//     if(planeMap!=nullptr)
//        planeMap->draw(painter,xpos, ypos, zoom);
     
     framerate.stop();
     
     this->set_avgRenderTime(framerate.avgFrameMsec);
     this->set_renderTime(framerate.frameMsec);

     return; 
}


void AirplaneDrawer::moveBy(qreal xOff, qreal yOff) {
    qreal ox, oy;

    //qDebug() << xOff <<","<<yOff;

    ox = position.x() - xOff;
    oy = position.y() - yOff;
//    if (ox < 0) ox = 0;
//    if (oy < 0) oy = 0;

    this->set_position(QPoint(ox,oy));   
}

bool AirplaneDrawer::zoomBy(qreal zoomFactor, qreal centerX, qreal centerY)
{
    auto newZoom = zoom + zoom * zoomFactor;
    if(newZoom>maxZoom || newZoom<minZoom) return false;
    zoom = newZoom;

    position -= QPoint( (centerX-position.x())*zoomFactor , (centerY-position.y())*zoomFactor);
    
    update();
    return true;
}


QPoint AirplaneDrawer::getMoveToCenterAt(QString id, qreal viewZoom)
{
    if(planeMap==nullptr) return QPoint(0,0);

//    auto item = planeMap->findItem(id);
    auto item = planeMap->findChair(id);
    if(item == nullptr)
        return position;
    
    if(viewZoom==0.0) viewZoom=this->zoom;
    
    auto itemCenter = item->location.center();
//    qDebug() << "zoom:" << viewZoom;
    auto itemOnScr = (itemCenter* viewZoom);
    auto tlWin = itemOnScr - boundingRect().center().toPoint();
    auto newPosition = -tlWin;

    return newPosition;
} 

QString AirplaneDrawer::getIdAt(int x, int y){
    QPoint p( (x - position.x())/ zoom, (y-position.y())/ zoom );
    auto item = planeMap->findItem(p);
    return (item!=nullptr)? item->id : "";
}
