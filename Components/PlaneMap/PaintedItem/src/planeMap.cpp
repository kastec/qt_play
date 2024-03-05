

#include <QObject>
#include "planeMap.h"
#include "Components/PlaneMap/PaintedItem/src/spriteCache.h"
//#include "qdatetime.h"
#include "planeLayoutParser.h"
#include "qguiapplication.h"


PlaneMap::PlaneMap()
{
  this->devicePixelRatio = qApp->devicePixelRatio();
}


void PlaneMap::createLayout(QList<QString> lines)
{
    PlaneLayoutParser layoutParser;
    auto items = layoutParser.parse(lines);
    planeItems = items;
    rows = layoutParser.rows;
    chairsInRow = layoutParser.maxChairsInRow;
    airplaneSize = layoutParser.getPlaneSize();
//    qDebug()<< "plane-layout-Size:" << airplaneSize ;
    
    planeSearcher.addItems(items, rows, chairsInRow);
    //    planeSearcher.print();
    
}


void PlaneMap::drawLayout(QPainter *painter, qreal zoom, QRect scrViewPort)
{
//   this->screenSize = painter->viewport().size() / this->devicePixelRatio;
//    QRect scrViewPort0 (-posOffset / zoom, screenSize / zoom);
    
//    qDebug()<<"scr:" << this->screenSize << "layout:"<<airplaneSize;
//    qDebug()<<"zoom" << zoom;
//    qDebug()<<"scrViewPort" << scrViewPort;
//    qDebug()<<"----";
    
    bool useBuffer=false;
    if(useBuffer)
        drawLayoutBuffer(painter, zoom, scrViewPort);
    else
        drawItems(painter, zoom, scrViewPort);
    
//    planeSearcher.dump();
}


void PlaneMap::drawLayoutBuffer(QPainter *painter, qreal zoom, QRect viewPort)
{
    QSize painterSize = painter->window().size();
    
    if(painterSize != buffer.bufferSize)
        buffer.setSize(painterSize);
    
    auto canUseBuffer = buffer.zoom==zoom && buffer.isInBuffer(viewPort);
 
    if(!canUseBuffer)
    {
        qDebug()<< "==[ REDRAW buffer]==";
        buffer.zoom = zoom;
        auto bufPainter = buffer.makePainter(viewPort);

        drawItems(bufPainter.data(), zoom, buffer.viewPort);
    }
    
    buffer.draw(painter, viewPort);
    return;
}

void PlaneMap::drawItems(QPainter *painter, qreal zoom, QRect viewPort)
{
//    auto ratio = painter->device()->devicePixelRatio();
    auto vpItems = planeSearcher.findItems(viewPort);
    
    auto painterOffset = -viewPort.topLeft() * zoom;
//    qDebug()<<"items to draw:"<< vpItems.length() << " viewPort:"<<viewPort;
    
    for (const auto item : vpItems) {
        
        auto l = item->location;
        auto pos = painterOffset + l.topLeft() * zoom;
        auto size = l.size() * zoom;
        
        QRect vpItemRect(pos, size);
                
        item->draw(painter, vpItemRect);
//        if(item->type=="seat"){
//            auto s = (PlaneItemChair*)item;
//            qDebug()<<"item "<< s->rowNumber <<s->letter << "loc:"<<item->location << "r:"<<vpItemRect;
//        }
        
    }     
}




void PlaneMap::drawNavMap(QPainter *painter, QRect scrViewPort)
{
    auto painterSize = painter->viewport().size() / this->devicePixelRatio;
    
    auto scale = __max(airplaneSize.width()/(qreal)painterSize.width(),
                       airplaneSize.height()/(qreal)painterSize.height());
    
    auto paintAreaSize = airplaneSize / scale;
    
    auto centerOffset = (painterSize.width()-paintAreaSize.width())/2;
    
    // границы борта
    auto painterArea = QRect(QPoint(centerOffset,0),paintAreaSize);
//    painter->drawRect(paintAircraft); // todo: remove - blact rect
        
    auto selectedColor = QBrush(Qt::black);
    auto defaultColor = QBrush(Qt::white);
    for(auto &i:planeItems)
    {
        if(i->type!="seat") continue;
        auto seat = (PlaneItemChair*)i;
        auto sLoc = seat->location.topLeft() / scale + QPoint(centerOffset,0);
        
        auto sSize =  seat->location.size() / (scale *1.2);

        QRect seatNav(sLoc, sSize);
  
        painter->fillRect(seatNav, seat->isSelected?selectedColor:defaultColor);        
    }
    
    // зона просмотра, выделение    
    QRect rect = QRect( scrViewPort.topLeft() /scale, scrViewPort.size() / (scale));
    this->navViewRect = rect.intersected(painterArea);
    
    QPen pen(QColor(0, 132, 255), 2.5);
    painter->setPen(pen);
    painter->fillRect(this->navViewRect, QBrush(QColor(169, 212, 251, 75)));
    painter->drawRect(this->navViewRect );    
}


PlaneItemChair* PlaneMap::findChair(const QString &seatNumber)
{
    return planeSearcher.findChair(seatNumber);    
}


PlaneItemBase* PlaneMap::findItem(QPoint p){
    return planeSearcher.find(p);
}

PlaneMap::~PlaneMap(){
    SpriteCache().clear();
    for(auto &pi : planeItems)
        delete pi;
    planeItems.clear();
}



















