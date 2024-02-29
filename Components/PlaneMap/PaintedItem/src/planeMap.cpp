
#include "planeMap.h"
#include "Components/PlaneMap/PaintedItem/src/spriteCache.h"
//#include "qdatetime.h"
#include "planeLayoutParser.h"

PlaneMap::PlaneMap()
{
}


void PlaneMap::createLayout(QList<QString> lines)
{
    PlaneLayoutParser layoutParser;
    auto items = layoutParser.parse(lines);
    planeItems = items;
    rows = layoutParser.rows;
    chairsInRow = layoutParser.maxChairsInRow;
    
    planeSearcher.addItems(items, rows, chairsInRow);
    //    planeSearcher.print(); 
}

// obsolete
void PlaneMap::draw(QPainter *painter, int xOff, int yOff, qreal zoom)
{
    QPoint offset(xOff, yOff);
    this->draw(painter, QPoint(xOff, yOff), zoom);
}

void PlaneMap::draw(QPainter *painter, QPoint posOffset, qreal zoom)
{
    QSize screenSize = QSize(painter->device()->width(),painter->device()->height());
    
    QRect scrViewPort (-posOffset / zoom, screenSize / zoom);
        
    bool useBuffer=false;
    if(useBuffer)
        drawBuffer(painter, posOffset, zoom, scrViewPort);
    else
        drawItems(painter, posOffset,  zoom, scrViewPort);
}


void PlaneMap::drawBuffer(QPainter *painter, QPoint offset, qreal zoom, QRect scrViewPort)
{
    QSize painterSize(painter->device()->width(),painter->device()->height());
    
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
    auto vpItems = planeSearcher.findItems(viewPort);
    
    for (const auto item : vpItems) {
        
        auto l = item->location;
        auto pos = painterOffset + l.topLeft() * zoom;
        auto size = l.size() * zoom;
        
        QRect vpItemRect(pos, size);       
        
        item->draw(painter, vpItemRect);
    }     
}


void PlaneMap::drawNavMap(QPainter *painter, int xOff, int yOff, qreal zoom)
{
//    QPoint offset(xOff, yOff);
//    QRect scrViewPort (-offset / zoom, screenSize / zoom);
    QRect rect(xOff, yOff, 80, 100);
    painter->fillRect(rect, QBrush(Qt::white));
//    painter->drawRect(30,30,80,100);

}

/* need to extract 
void PlaneMap::drawItems(QPainter *painter, int xOff, int yOff, qreal zoom)
{
     QRect viewPort (-xOff / zoom, -yOff / zoom, screenSize.width() / zoom,screenSize.height()/zoom);
     auto vpItems = planeSearcher.getItemsInRect(viewPort);
     
for (const auto item : vpItems) {

auto l = &item->location;

int vpX = xOff + l->left() * zoom;
int vpY = yOff + l->top() * zoom;
int vpW = l->width() * zoom;
int vpH = l->height() * zoom;

QRect vpItemRect(vpX,vpY,vpW,vpH);       
item->draw(painter, vpItemRect);
}     
}
*/
/*
QList<PlaneItemBase*> PlaneMap::getItemsInViewPort(int xOff, int yOff, qreal zoom){
     
QList<PlaneItemBase*> vpItems;
QRect viewPort (-xOff / zoom, -yOff / zoom, screenSize.width() / zoom,screenSize.height()/zoom);

vpItems =planeSearcher.getItemsInRect(viewPort);
 
//        QElapsedTimer t;
//        t.start();  
//     for(const auto &item : planeItems)
//        if(item->location.intersects(viewPort)){
//            vpItems.append(item);
//        }

//      qDebug() <<t.nsecsElapsed() << t.elapsed();

return vpItems;
}*/

PlaneItemChair* PlaneMap::findChair(const QString &seatNumber)
{
    return planeSearcher.findChair(seatNumber);    
}

//PlaneItemBase* PlaneMap::findItem(const QString &objId){
    //    for(const auto &item : planeItems)
    //        if(item->id==id) return item;
    //    return nullptr;
//    return planeSearcher.find(objId);    
//}

PlaneItemBase* PlaneMap::findItem(QPoint p){
    return planeSearcher.find(p);
}

PlaneMap::~PlaneMap(){
    SpriteCache().clear();
    for(auto &pi : planeItems)
        delete pi;
    planeItems.clear();
}



















