
#ifndef PLANEMAP_H
#define PLANEMAP_H

#include "planeItems/planeItemBase.h"
#include "planeLayoutSearcher.h"
#include "renderBuffer.h"
//#include "planeNavDrawer.h"

class PlaneNavDrawer;

class PlaneMap
{
  friend class PlaneNavDrawer;
    
  private:
 
    QList<PlaneItemBase*> planeItems;
    
    //OBSOLETE - DELTE  для правильного расчета размеров на Windows
    qreal devicePixelRatio;
    
    PlaneNavDrawer *navDrawer;
    
  public:
    PlaneLayoutSearcher planeSearcher;
    
  public:
    PlaneMap();
    int rows;
    QSize layoutSize;
        
    void createLayout(QList<QString> lines);
    
    void drawLayout(QPainter *painter, qreal zoom, QRect scrViewPort);    
    void drawNavMap(QPainter *painter, QRect scrViewPort);
    
//    void setScreenSize(QSize screenSize);
    
    PlaneItemChair* findChair(const QString &seatNumber);    
    PlaneItemBase* findItem(const QString &objId);
    PlaneItemBase* findItem(QPoint point);
    
    std::pair<const QRect&, const QRect&> getNavPaintSizes();
   private:
    
    RenderBuffer buffer;
     
    void drawLayoutBuffer(QPainter *painter, qreal zoom, QRect scrViewPort);
    void drawItems(QPainter *painter, qreal zoom, QRect viewPort);
    
  public:
    ~PlaneMap();
};


#endif // PLANEMAP_H
