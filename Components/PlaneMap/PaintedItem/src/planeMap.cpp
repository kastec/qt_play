

#include <QObject>
#include "planeMap.h"
#include "Components/PlaneMap/PaintedItem/src/spriteCache.h"
//#include "qdatetime.h"
#include "planeLayoutParser.h"
#include "qguiapplication.h"

#include "planeNavDrawer.h"

PlaneMap::PlaneMap()
{
    this->devicePixelRatio = qApp->devicePixelRatio();
    layoutSize = QSize(0,0);
    this->navDrawer = new PlaneNavDrawer(this);
}


void PlaneMap::createLayout(QList<QString> lines)
{
    PlaneLayoutParser layoutParser;
    auto items = layoutParser.parse(lines);
    planeItems = items;
    rows = layoutParser.rows;
    chairsInRow = layoutParser.maxChairsInRow;
    layoutSize = layoutParser.getPlaneSize();
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
    // просто вынесл логику отрисовки в отдельный класс, чтобы не было простыни
    this->navDrawer->drawNavMap(painter, scrViewPort);
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
    
    if(this->navDrawer!=nullptr)
        delete this->navDrawer;
}



















