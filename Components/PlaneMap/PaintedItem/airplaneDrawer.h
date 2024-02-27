
#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>

#include <QObject>
#include "Utils/JsonHelper/qqmlhelper.h"
#include "./src/planeMap.h"
#include "./src/frameRateCalc.h"

// https://web.archive.org/web/20220517014056/https://www.huber.xyz/?p=477
class AirplaneDrawer : public QQuickPaintedItem
{
    Q_OBJECT
    
    QML_WRITABLE_PROPERTY(int, avgRenderTime);
    QML_WRITABLE_PROPERTY(int, renderTime);
    
    QML_WRITABLE_PROPERTY(PlaneMap*, planeMap);
   
    QML_WRITABLE_PROPERTY_CB(QPoint, position, onPositionChanging);
    QML_WRITABLE_PROPERTY_CB(qreal, zoom, onZoomChanging);
 
  public:
    AirplaneDrawer(QQuickItem *parent = nullptr);
   
    void paint(QPainter *painter);

   // QImage image() const;
    
    Q_INVOKABLE void moveBy(qreal xOff, qreal yOff);
    Q_INVOKABLE bool zoomBy(qreal z, qreal centerX, qreal centerY);
    
    Q_INVOKABLE void changeVisibleSize(QSize size);

    Q_INVOKABLE QPoint getMoveToCenterAt(QString id, qreal zoom=0.0);
    Q_INVOKABLE QString getIdAt(int x, int y);
    
    bool onPositionChanging(QPoint newPos);
    bool onZoomChanging(qreal newZoom);
    
    
  private:
//    bool zoomInternal(qreal zoomFactor, qreal centerX, qreal centerY);
    
  private:
    bool zoomScreenCenter;
    
    qreal maxZoom, minZoom;
    QSize screenSize;

    FrameRateCalc framerate;

};

#endif // IMAGEITEM_H
