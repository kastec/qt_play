
#ifndef PLANENAVDRAWER_H
#define PLANENAVDRAWER_H

#include <QObject>
#include "planeItemExit.h"
#include "qpainter.h"
#include "drawHelper.h"
#include "planeMap.h"
#include "drawHelper.h"

class PlaneNavDrawer
{

  private:
    PlaneMap  *planeMap;
    
  public:
    PlaneNavDrawer(PlaneMap *planeMap):planeMap(planeMap){}
    
    QRect drawNavMap(QPainter *painter, QRect scrViewPort)
    {
        auto painterSize = painter->viewport().size() / planeMap->devicePixelRatio;
        
        auto loyoutSize = planeMap->layoutSize;
        auto scale = __max(loyoutSize.width()/(qreal)painterSize.width(),
                           loyoutSize.height()/(qreal)painterSize.height());
        
        auto paintAreaSize = loyoutSize / scale;
        
        // смещение для выравнивания по центру
        auto centerOffset = QPoint( (painterSize.width()-paintAreaSize.width())/2 , 0);
        
        // границы борта
        auto painterArea = QRect(centerOffset, paintAreaSize);
        
        // отрисовка сидений
        drawItems(painter, scale, centerOffset);
        
        // зона просмотра, выделение
        QRect rect = QRect( scrViewPort.topLeft() /scale + centerOffset, scrViewPort.size() / (scale));
        auto navViewRect = rect.intersected(painterArea);
        
        QPen pen(QColor(0, 132, 255), 2.5);
        painter->setPen(pen);
        
        painter->fillRect(navViewRect, QBrush(QColor(169, 212, 251, 75)));
        painter->drawRect(navViewRect.marginsAdded(QMargins(-1,0,-1,0))); // margin добавляем из-за толшины линии обводки
        
        return navViewRect;
    }
    
    void drawItems(QPainter *painter,  qreal scale, QPoint centerOffset)
    {        
        for(auto &i:planeMap->planeItems)
        {
            if(i->type=="seat")
                drawSeat(painter, (PlaneItemChair*)i, scale, centerOffset);
            
            if(i->type=="exit")
                drawExit(painter, (PlaneItemExit*)i, scale, centerOffset);      
        }
    }
    
    void drawSeat(QPainter *painter, PlaneItemChair *chair,  qreal scale, QPoint centerOffset)
    {
        auto sLoc = chair->location.topLeft() / scale + centerOffset;
        
        // уменьшим масштаб на 1.2, чтобы лучше отделить сиденья друг от друга
        auto sSize =  chair->location.size() / (scale *1.2);            
        QRect seatNav(sLoc, sSize);            
        painter->fillRect(seatNav,  chair->isSelected?QColorConstants::Black:QColorConstants::White);   
    }
    
    void drawExit(QPainter *painter, PlaneItemExit *exit,  qreal scale, QPoint centerOffset)
    {
        auto sLoc = exit->location.topLeft() / scale + centerOffset;
        auto sSize =  exit->location.size() / (scale);   
          
        auto w = sSize.width();
        auto h = sSize.height();
        QPen lineColor(Qt::red, h*0.2);
        painter->setPen(lineColor);
        
        QList<QPoint> points;
        auto wlen = w*0.2;
        if(exit->isLeft)
        {
            points.append(sLoc+QPoint(wlen,2));
            points.append(sLoc+QPoint(2,2));
            points.append(sLoc+QPoint(2,h-2));
            points.append(sLoc+QPoint(wlen,h-2));
        }
        else
        {
            points.append(sLoc+QPoint(w-wlen,2));
            points.append(sLoc+QPoint(w-2,2));
            points.append(sLoc+QPoint(w-2,h-2));
            points.append(sLoc+QPoint(w-wlen,h-2)); 
        }
        painter->drawPolyline(points.data(),4);
    }
};


#endif // PLANENAVDRAWER_H






