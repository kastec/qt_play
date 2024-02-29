
#ifndef AIRPLANEVIEWMODEL_H
#define AIRPLANEVIEWMODEL_H

#include <QObject>
#include <QQuickPaintedItem>
#include "Utils/JsonHelper/qqmlhelper.h"
#include "./src/planeMap.h"
#include "paintArea.h"

class AirplaneViewModel: public QObject
{
    Q_OBJECT
    
    QML_WRITABLE_PROPERTY(PlaneMap*, planeMap);
    
    QML_WRITABLE_PROPERTY(int, avgRenderTime);
    QML_WRITABLE_PROPERTY(int, renderTime);
    
  private:
    
    qreal zoom; 
    QPoint position;
    QRect scrViewPort;
    
    PaintArea *airplanePainter;
    PaintArea *navigationPainter;
    
  public:
    explicit AirplaneViewModel(QObject *parent = nullptr);
    
    Q_INVOKABLE void loadLayout();
    void setPassengers(const QList<int> &passengers);
    void setSelections(const QList<QString> &selectedSeats);
    
    
    Q_INVOKABLE void addPainter(QString name, PaintArea *painter);
    
    Q_INVOKABLE void moveBy(qreal xOff, qreal yOff);
    Q_INVOKABLE bool zoomBy(qreal z, qreal centerX, qreal centerY);
    
  private:
  
    void drawAirplaneLayout(QPainter *painter);
    void updatePaintArea();
    
    public:
    ~AirplaneViewModel();
};

#endif // AIRPLANEVIEWMODEL_H
