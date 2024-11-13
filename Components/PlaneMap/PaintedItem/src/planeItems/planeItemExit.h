
#ifndef PLANEITEMEXIT_H
#define PLANEITEMEXIT_H


#include "planeItemBase.h"

class PlaneItemExit : public PlaneItemBase
{
  public:
    bool isLeft;
    bool isShort;
    
    PlaneItemExit():PlaneItemBase("exit"){
    }
    
    void draw(QPainter *p, QRect &rect);
    
  private:
    static const QList<QLineF> exitLines;
    static  QList<QLineF> fillExitLines();
    
    QPixmap* makePixmapExit(int width, int height);
    QPixmap* makePixmapDoor(int height, bool isLeft);
};

#endif // PLANEITEMEXIT_H
