
#ifndef PLANEITEMROWNUMBER_H
#define PLANEITEMROWNUMBER_H

#include <QObject>
#include <QPainter>
#include "planeItemBase.h"

class PlaneItemRowNumber : public PlaneItemBase
{
  public:
    QColor color;
    int fontSize;
    
  public:   
    PlaneItemRowNumber():PlaneItemBase("rownum"){  }    
    void draw(QPainter *p, QRect &rect);
};

#endif // PLANEITEMROWNUMBER_H
