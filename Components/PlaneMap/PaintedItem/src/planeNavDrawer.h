
#ifndef PLANENAVDRAWER_H
#define PLANENAVDRAWER_H

#include <QObject>
#include "qpainter.h"

#include "planeMap.h"

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
        auto centerOffset = (painterSize.width()-paintAreaSize.width())/2;
        
        // границы борта
        auto painterArea = QRect(QPoint(centerOffset,0),paintAreaSize);
        //    painter->drawRect(paintAircraft); // todo: remove - blact rect
        
        auto selectedColor = QBrush(Qt::black);
        auto defaultColor = QBrush(Qt::white);
        for(auto &i:planeMap->planeItems)
        {
            if(i->type!="seat") continue;
            auto seat = (PlaneItemChair*)i;
            auto sLoc = seat->location.topLeft() / scale + QPoint(centerOffset,0);
            
            auto sSize =  seat->location.size() / (scale *1.2);
            
            QRect seatNav(sLoc, sSize);
            
            painter->fillRect(seatNav, seat->isSelected?selectedColor:defaultColor);        
        }
        
        // зона просмотра, выделение
        QRect rect = QRect( scrViewPort.topLeft() /scale + QPoint(centerOffset,0), scrViewPort.size() / (scale));
        auto navViewRect = rect.intersected(painterArea);
        
        QPen pen(QColor(0, 132, 255), 2.5);
        painter->setPen(pen);
        
        painter->fillRect(navViewRect, QBrush(QColor(169, 212, 251, 75)));
        painter->drawRect(navViewRect.marginsAdded(QMargins(-1,0,-1,0))); // margin добавляем из-за толшины линии обводки
        
        return navViewRect;
    }    

};

#endif // PLANENAVDRAWER_H
