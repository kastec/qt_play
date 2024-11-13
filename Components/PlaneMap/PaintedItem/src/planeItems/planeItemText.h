
#ifndef PLANEITEMTEXT_H
#define PLANEITEMTEXT_H

#include <QObject>
#include <QPainter>
#include "planeItemBase.h"

class PlaneItemText : public PlaneItemBase
{
  public:
    QColor color;
    int fontSize;
    
  public:
    PlaneItemText():PlaneItemBase("text"), color(Qt::black), fontSize(20){  }    
    void draw(QPainter *p, QRect &rect);
    
  private:
    void drawText(QPainter *painter, QRect &rect);
};


#endif // PLANEITEMTEXT_H
