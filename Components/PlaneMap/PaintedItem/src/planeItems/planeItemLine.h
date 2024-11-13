
#ifndef PLANEITEMLINE_H
#define PLANEITEMLINE_H

#include <QObject>
#include <QPainter>
#include "planeItemBase.h"


class PlaneItemLine: public PlaneItemBase
{
  public:
    QColor color;
    int width;
    

  public:
    PlaneItemLine():PlaneItemBase("line"), color(Qt::black), width(2){  }    
    void draw(QPainter *p, QRect &rect);
    
};

#endif // PLANEITEMLINE_H
