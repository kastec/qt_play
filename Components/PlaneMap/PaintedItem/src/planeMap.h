
#ifndef PLANEMAP_H
#define PLANEMAP_H

#include "planeItemBase.h"
//#include "planeItemChair.h"
#include "planeLayoutSearcher.h"
#include "renderBuffer.h"


class PlaneMap
{
    
  private:
    QSize screenSize;  
    QList<PlaneItemBase*> planeItems;
    
    // для правильного расчета размеров на Windows
    qreal devicePixelRatio;
    
  public:
    PlaneLayoutSearcher planeSearcher;
    
  public:
    PlaneMap();
    int rows, chairsInRow;
    QSize airplaneSize;
    
    void createLayout(QList<QString> lines);
    // obsolete
//    void draw(QPainter *painter, int xOff, int yOff, qreal zoom);
    
    void drawLayout(QPainter *painter, QPoint posOffset, qreal zoom);    
    void drawNavMap(QPainter *painter, QPoint posOffset, qreal zoom);
        
    void setScreenSize(QSize screenSize);
    
    PlaneItemChair* findChair(const QString &seatNumber);    
    PlaneItemBase* findItem(const QString &objId);
    PlaneItemBase* findItem(QPoint point);
    
//    PlaneItemChair* findChair(QString seatNumber);
    
   private:
//    QList<PlaneItemBase*> getItemsInViewPort(int xOff, int yOff, qreal zoom);
    
    RenderBuffer buffer;
     
     void drawLayoutBuffer(QPainter *painter, QPoint offset, qreal zoom, QRect scrViewPort);
    void drawItems(QPainter *painter, QPoint painterOffset, qreal zoom, QRect viewPort);
    
  public:
    ~PlaneMap();
};


#endif // PLANEMAP_H
