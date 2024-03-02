
#include "airplaneNavDrawer.h"


AirplaneNavDrawer::AirplaneNavDrawer(QQuickItem *parent) : QQuickPaintedItem(parent), position(QPoint(0,0))
{  
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    
    //TODO: load planeLayoutDescription, pass it to createLayout
    //    planeMap = new PlaneMap();
    //    planeMap->createLayout();
}

// --- Props

bool AirplaneNavDrawer::onPositionChanging(QPoint newPos){
    position = newPos;
    //  qDebug() <<"pos:"<< newPos ;
//    update();
    return true;
}

void AirplaneNavDrawer::setMapPosition(QPoint pos)
{
    this->set_position(pos);
    update();
}
void AirplaneNavDrawer::paint(QPainter *painter)
{
    auto xpos = position.x();
    auto ypos = position.y();
    
   
    // PLANE RENDER
//    if(planeMap!=nullptr)
//        planeMap->drawNavMap(painter,xpos, ypos, 1.0);
   
    return; 
}
