
#ifndef PLANENAVDRAWER_H
#define PLANENAVDRAWER_H

#include <QObject>
#include "planeItemExit.h"
#include "spriteCache.h"
#include "qpainter.h"
#include "planeMap.h"

class PlaneNavDrawer
{    
  private:
    PlaneMap  *planeMap;
    
  public:
    PlaneNavDrawer(PlaneMap *planeMap):planeMap(planeMap){}    
    
    void drawNavMap(QPainter *painter, QRect scrViewPort)
    {
        auto painterSize = painter->viewport().size() / planeMap->devicePixelRatio;
        
        auto layoutSize = planeMap->layoutSize;
        auto scale = __max(layoutSize.width()/(qreal)painterSize.width(),
                           layoutSize.height()/(qreal)painterSize.height());
        
        auto paintAreaSize = layoutSize / scale;
        
        // смещение для выравнивания по центру
        auto centerOffset = QPoint( (painterSize.width()-paintAreaSize.width())/2 , 0);
        
        static QString spriteKey("nav-map");
        auto cacheCriteria = paintAreaSize.width()*paintAreaSize.height();
        
        auto navPixmap = SpriteCache().get(spriteKey,cacheCriteria);
        if(!navPixmap){
            // кеш отрисованной карты навигации - перерисовка при изменении размеров
            navPixmap = new QPixmap(paintAreaSize);
            navPixmap->fill(QColor::fromRgb(217,230,237));
            QPainter pNavItems(navPixmap);
            
            // отрисовка сидений, выходов
            drawItems(&pNavItems, scale, QPoint(0,0) /*centerOffset*/);
            
            SpriteCache().push(spriteKey, navPixmap, cacheCriteria);            
        }
        
        painter->drawPixmap(centerOffset, *navPixmap);
        
        // зона просмотра, выделение
        drawViewPortArea(painter, scrViewPort,  centerOffset,  paintAreaSize, scale);
    }
    
    
    void drawViewPortArea(QPainter *painter, QRect scrViewPort, QPoint centerOffset, QSize paintAreaSize, qreal scale)
    {
        // границы борта
        auto painterArea = QRect(centerOffset, paintAreaSize);
        
        QRect rect = QRect( scrViewPort.topLeft() /scale + centerOffset, scrViewPort.size() / (scale));
        
        auto navViewRect = rect.intersected(painterArea);
        
        QPen pen(QColor(0, 132, 255), 2.5);
        painter->setPen(pen);
        
        painter->fillRect(navViewRect, QBrush(QColor(169, 212, 251, 75)));
        painter->drawRect(navViewRect.marginsAdded(QMargins(-1,0,-1,0))); // margin добавляем из-за толшины линии обводки
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
        
        // сделаем небольшой отступ от края, чтобы рамка ViewPort не перекрывала выход 
        auto offset = sSize.height()*0.2;
        sLoc = sLoc + QPoint((exit->isLeft)?offset:-offset,0);
        
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






