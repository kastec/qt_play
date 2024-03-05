
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
    
    //OBSOLETE - DELTE  для правильного расчета размеров на Windows
    qreal devicePixelRatio;
    
  public:
    PlaneLayoutSearcher planeSearcher;
    
  public:
    PlaneMap();
    int rows, chairsInRow;
    QSize airplaneSize;
    QRect navViewRect;
    
    void createLayout(QList<QString> lines);
    // obsolete
//    void draw(QPainter *painter, int xOff, int yOff, qreal zoom);
    
    void drawLayout(QPainter *painter, qreal zoom, QRect scrViewPort);    
    void drawNavMap(QPainter *painter, QRect scrViewPort);
        
    void setScreenSize(QSize screenSize);
    
    PlaneItemChair* findChair(const QString &seatNumber);    
    PlaneItemBase* findItem(const QString &objId);
    PlaneItemBase* findItem(QPoint point);
    
//    PlaneItemChair* findChair(QString seatNumber);
    
   private:
//    QList<PlaneItemBase*> getItemsInViewPort(int xOff, int yOff, qreal zoom);
    
    RenderBuffer buffer;
     
    void drawLayoutBuffer(QPainter *painter, qreal zoom, QRect scrViewPort);
    void drawItems(QPainter *painter, qreal zoom, QRect viewPort);
    
  public:
    ~PlaneMap();
};


#endif // PLANEMAP_H
