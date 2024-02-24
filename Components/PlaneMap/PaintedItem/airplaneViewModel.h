
#ifndef AIRPLANEVIEWMODEL_H
#define AIRPLANEVIEWMODEL_H

#include <QObject>
#include "Utils/JsonHelper/qqmlhelper.h"
#include "./src/planeMap.h"

class AirplaneViewModel: public QObject
{
    Q_OBJECT
    
    QML_WRITABLE_PROPERTY(PlaneMap*, planeMap);
    
    QML_WRITABLE_PROPERTY(int, avgRenderTime);
    QML_WRITABLE_PROPERTY(int, renderTime);
    
  public:
    explicit AirplaneViewModel(QObject *parent = nullptr);
    
    Q_INVOKABLE void loadLayout();
    void setPassengers(const QList<int> &passengers);
    void setSelections(const QList<QString> &selectedSeats);
    
  private:

    
    public:
    ~AirplaneViewModel();
};

#endif // AIRPLANEVIEWMODEL_H
