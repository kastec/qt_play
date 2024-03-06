
#include "planeItemExit.h"
#include <QMargins>
#include "spriteCache.h"
#include "qdebug.h"


const QList<QLineF> PlaneItemExit::exitLines = PlaneItemExit::fillExitLines();


QList<QLineF> PlaneItemExit::fillExitLines()
{
     auto exitlines = QList<QLine>{
        QLine(3,3, 22,3),
        QLine(3,3, 3,71),
        QLine(3,71, 22,71),
        QLine(3,35, 19,35),
        QLine(28,3, 57,71),
        QLine(28,71, 57,3),
        QLine(65,3, 65,71 ),
        QLine(73,3, 99,3),
        QLine(85,3, 85,71)
    } ;
    
    auto scaledLines = PlaneItemBase::rescale(exitlines, 100,73);
     
    return scaledLines;
}


void PlaneItemExit::draw(QPainter *painter, QRect &rect)
{
    auto w = rect.width(), h = rect.height();
    auto x = rect.x(), y = rect.y();
    
    auto door = makePixmapDoor(h, this->isLeft);
    
    auto exW = w*0.8, exH = h*0.8;
    
    auto exit = makePixmapExit(exW,exH);

    auto doorW = door->width();
    auto space = doorW*1.5;
    
    if(this->isLeft)    {
        painter->drawPixmap(x,y, *door);
        painter->drawPixmap(x + doorW + space, y+h*0.1, *exit);
    }
    else
    {
        painter->drawPixmap(x, y+h*0.1, *exit);
        painter->drawPixmap(x+exit->width()+space/2,y, *door);    
    }
}


QPixmap* PlaneItemExit::makePixmapDoor(int height, bool isLeft)
{
    auto spriteKey = isLeft?"exit-[":"ext-]";
    auto spite = SpriteCache().get(spriteKey, height);
    if(spite)
        return spite;
    
    int width = height *0.1;
    QPixmap* pixmap = new QPixmap(width, height);
    pixmap->fill();
    
    QPainter p(pixmap);
    
    //     p.setRenderHint(QPainter::Antialiasing);
    QPen lineColor(Qt::black, height*0.04);
    p.setPen(lineColor);
      
    
    QList<QPoint> points;
    if(isLeft)
    {
        points.append(QPoint{width,2});
        points.append(QPoint{2,2});
        points.append(QPoint{2,height-2});
        points.append(QPoint{width,height-2});
    }
    else
    {
        points.append(QPoint{2,2});
        points.append(QPoint{width-2,2});
        points.append(QPoint{width-2,height-2});
        points.append(QPoint{2,height-2}); 
    }
    p.drawPolyline(points.data(),4);
//        QPoint points[4]{ QPoint{width,2}, QPoint{2,2}, QPoint{2,height-2}, QPoint{width,height-2}};
    //p.drawPolyline(points,4);
  
    p.end();
    
    SpriteCache().push(spriteKey, pixmap, height);
    return pixmap;
}

QPixmap* PlaneItemExit::makePixmapExit(int width, int height)
{
    QString spriteKey("exit-text");
    auto spite = SpriteCache().get(spriteKey, height);
    if(spite)
        return spite;
    
//    qDebug()<< "exit: " <<width<<height;
    QPixmap* pixmap = new QPixmap(width, height);
    pixmap->fill();
  
    QPainter p(pixmap);
    
//    p.fillRect(0,0,  width, height, QColor(150,99,99));
    
    p.setRenderHint(QPainter::Antialiasing);
    QPen exitColor(QColor(231,136,149), width*0.035);
    p.setPen(exitColor);
    
    
    QList<QLine> scaledLines;
    for(auto &l: exitLines){
        scaledLines.append(QLine(l.x1()*width, l.y1()*height, l.x2()*width, l.y2()*height));
    }

    p.drawLines(scaledLines);

    p.end();
    
    SpriteCache().push(spriteKey, pixmap, height);
    return pixmap;
}
