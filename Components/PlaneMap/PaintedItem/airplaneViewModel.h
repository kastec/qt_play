
#ifndef AIRPLANEVIEWMODEL_H
#define AIRPLANEVIEWMODEL_H

#include <QObject>
#include <QQuickPaintedItem>
#include "./src/frameRateCalc.h"
#include "Utils/JsonHelper/qqmlhelper.h"
#include "./src/planeMap.h"
#include "paintArea.h"

class AirplaneViewModel: public QObject
{
    Q_OBJECT
    
    QML_WRITABLE_PROPERTY(PlaneMap*, planeMap);
    
    QML_WRITABLE_PROPERTY(int, avgRenderTime);
    QML_WRITABLE_PROPERTY(int, renderTime);
    
//    QML_WRITABLE_PROPERTY(QPoint, position);
//    QML_WRITABLE_PROPERTY(qreal, zoom);
    QML_WRITABLE_PROPERTY_CB(QPoint, position, onPositionChanging);
    QML_WRITABLE_PROPERTY_CB(qreal, zoom, onZoomChanging);
 
    
  private:    
 
//    QRect scrViewPort;
    bool isFixedZoom;
    qreal maxZoom, minZoom;
    
    QSize screenSize;
    
    PaintArea *airplanePainter;
    PaintArea *navigationPainter;
    
    FrameRateCalc framerate;
    
    // для правильного расчета размеров на Windows
    qreal devicePixelRatio;
    
  public:
    explicit AirplaneViewModel(QObject *parent = nullptr);
    
    Q_INVOKABLE void loadLayout();
    Q_INVOKABLE void changeVisibleSize(QSize size);
    
    void setPassengers(const QList<int> &passengers);
    void setSelections(const QList<QString> &selectedSeats);
    
    
    Q_INVOKABLE void addPainter(QString name, PaintArea *painter);
    
    Q_INVOKABLE void moveBy(qreal xOff, qreal yOff);
    Q_INVOKABLE bool zoomBy(qreal z, qreal centerX, qreal centerY);
    
    Q_INVOKABLE QPoint getMoveToCenterAt(QString id, qreal zoom=0.0);
    Q_INVOKABLE QString getIdAt(int x, int y);
    
    Q_INVOKABLE void moveNavBy(qreal xOff, qreal yOff);
    Q_INVOKABLE void setNavPos(qreal xOff, qreal yOff);
    
  private:
    bool onPositionChanging(QPoint newPos);
    bool onZoomChanging(qreal newZoom);
    
    void changeZoomLimits();
    
    
    QRect getSrcViewPort();
    qreal getNavMapScale();
    void drawAirplaneLayout(QPainter *painter);
    void drawNavigation(QPainter *painter);
    void updatePaintArea();

    public:
    ~AirplaneViewModel();
};

#endif // AIRPLANEVIEWMODEL_H
