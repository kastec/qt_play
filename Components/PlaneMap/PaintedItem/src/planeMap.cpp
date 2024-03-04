

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


void PlaneMap::drawLayout(QPainter *painter, QPoint posOffset, qreal zoom, QRect scrViewPort)
{
//   this->screenSize = painter->viewport().size() / this->devicePixelRatio;
//    QRect scrViewPort0 (-posOffset / zoom, screenSize / zoom);
    
//    qDebug()<<"scr:" << this->screenSize << "layout:"<<airplaneSize;
//    qDebug()<<"zoom" << zoom;
//    qDebug()<<"scrViewPort" << scrViewPort;
//    qDebug()<<"----";
    
    bool useBuffer=false;
    if(useBuffer)
        drawLayoutBuffer(painter, posOffset, zoom, scrViewPort);
    else
        drawItems(painter, zoom, scrViewPort);
    
//    planeSearcher.dump();
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
        drawItems(bufPainter.data(),  zoom, buffer.viewPort);
    }
    
    buffer.draw(painter, scrViewPort);
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


void PlaneMap::drawNavMap(QPainter *painter, QPoint posOffset, qreal zoom, QRect scrViewPort)
{
    auto painterSize = painter->viewport().size() / this->devicePixelRatio;
    

    auto scaleH = airplaneSize.height()/(qreal)painterSize.height();
    
    auto paintWidth =  airplaneSize.width()/scaleH;
//    qDebug()<< "nav H:" <<  painterSize.height() <<"airplaneSize"<< airplaneSize << "scale:"<<scaleW<<"x"<<scaleH;
//    QRect scrViewPort (posOffset / zoom, this->screenSize / zoom);
    
    // границы борта
    painter->drawRect(0,0,paintWidth,painterSize.height());
    
    // зона просмотра, выделение
    QRect rect = QRect( scrViewPort.topLeft() /scaleH, scrViewPort.size() / (scaleH));
    painter->fillRect(rect, QBrush(Qt::white));
    
    //    qDebug()<<"nav:" << painterSize<< "scr:" << this->screenSize << "layout:"<<airplaneSize;
    //    qDebug()<<"scale" << scale;
    //    qDebug()<<"scrViewPort" << scrViewPort;
    //    qDebug()<<"rect" << rect;
    //    qDebug()<<"----";
    
     
    for(auto &i:planeItems)
    {
        if(i->type!="seat") continue;
        auto seat = (PlaneItemChair*)i;
        auto sLoc = seat->location.topLeft() / scaleH;
        auto sSize =  seat->location.size() / (scaleH *1.2);

        QRect seatNav(sLoc, sSize);
        painter->fillRect(seatNav, QBrush(Qt::blue));
        
    }

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



















