
#ifndef AIRPLANENAVDRAWER_H
#define AIRPLANENAVDRAWER_H

#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>

#include "qquickpainteditem.h"
#include <QObject>

#include "Utils/JsonHelper/qqmlhelper.h"

#include "./src/planeMap.h"


class AirplaneNavDrawer : public QQuickPaintedItem
{
    Q_OBJECT
    
    QML_WRITABLE_PROPERTY(PlaneMap*, planeMap);
    QML_WRITABLE_PROPERTY(QPoint, position);
 //   QML_WRITABLE_PROPERTY_CB(QPoint, position, onPositionChanging);
  public:
    
    AirplaneNavDrawer(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);
    
    bool onPositionChanging(QPoint newPos);
    
    Q_INVOKABLE void setMapPosition(QPoint pos);
};

#endif // AIRPLANENAVDRAWER_H
