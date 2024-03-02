

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


void PlaneMap::drawLayout(QPainter *painter, QPoint posOffset, qreal zoom)
{
    this->screenSize = painter->viewport().size() / this->devicePixelRatio;
  
    QRect scrViewPort (-posOffset / zoom, screenSize / zoom);
    
//    qDebug()<<"scr:" << this->screenSize << "layout:"<<airplaneSize;
//    qDebug()<<"zoom" << zoom;
//    qDebug()<<"scrViewPort" << scrViewPort;
//    qDebug()<<"----";
    
    bool useBuffer=false;
    if(useBuffer)
        drawLayoutBuffer(painter, posOffset, zoom, scrViewPort);
    else
        drawItems(painter, posOffset,  zoom, scrViewPort);

}


void PlaneMap::drawLayoutBuffer(QPainter *painter, QPoint offset, qreal zoom, QRect scrViewPort)
{
    QSize painterSize = painter->window().size();
    
    if(painterSize != buffer.bufferSize)
        buffer.setSize(painterSize);
    
    auto canUseBuffer = buffer.zoom==zoom && buffer.isInBuffer(scrViewPort);
 
    if(!canUseBuffer)
    {
        qDebug()<< "==[ REDRAW buffer]==";
        buffer.zoom = zoom;
        auto bufPainter = buffer.makePainter(scrViewPort);        
        auto offs = offset - (buffer.viewPort.topLeft() - scrViewPort.topLeft()) *zoom;
        drawItems(bufPainter.data(), offs,  zoom, buffer.viewPort);
    }
    
    buffer.draw(painter, scrViewPort);
    return;
}

void PlaneMap::drawItems(QPainter *painter, QPoint painterOffset, qreal zoom, QRect viewPort)
{
//    auto ratio = painter->device()->devicePixelRatio();
    auto vpItems = planeSearcher.findItems(viewPort);
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


void PlaneMap::drawNavMap(QPainter *painter, QPoint posOffset, qreal zoom)
{
    auto painterSize = painter->viewport().size() / this->devicePixelRatio;
    auto scale = airplaneSize.height()/(qreal)painterSize.height();
    
    QRect scrViewPort (posOffset / zoom, this->screenSize / zoom);

    QRect rect = QRect( -scrViewPort.topLeft() /scale, scrViewPort.size()/scale);
//    qDebug()<<"nav:" << painterSize<< "scr:" << this->screenSize << "layout:"<<airplaneSize;
//    qDebug()<<"scale" << scale;
//    qDebug()<<"scrViewPort" << scrViewPort;
//    qDebug()<<"rect" << rect;
//    qDebug()<<"----";

    painter->fillRect(rect, QBrush(Qt::white));

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



















