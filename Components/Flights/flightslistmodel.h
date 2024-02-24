
#ifndef FLIGHTSLISTMODEL_H
#define FLIGHTSLISTMODEL_H


//#include "Utils/HttpRequester/requester.h"
#include "Utils/JsonHelper/qqmlhelper.h"
//#include "qdatetime.h"
//#include <QQmlHelper>
#include <QObject>
#include "flight.h"
#include "qjsonarray.h"

class FlightsListModel : public QObject {
    Q_OBJECT
    QML_WRITABLE_PROPERTY(QList<Flight *>, fligths);
    QML_WRITABLE_PROPERTY(QJsonArray, jsFligths);
    
//    QML_WRITABLE_PROPERTY(QJsonObject*, jsCurrentFlight);
//    QML_WRITABLE_PROPERTY(Flight*, currentFlight);
    
    QML_WRITABLE_PROPERTY(Flight*, curFlight);
    QML_WRITABLE_PROPERTY(QVariant, qvarFlight);

  public:

    explicit FlightsListModel(QObject *parent = nullptr);
    
    Q_INVOKABLE void loadJsonArray();
    Q_INVOKABLE void loadJsonArray2();
    Q_INVOKABLE void saveFlights();
 
  private:
    
    void loadJsonArrayUrl(QString api);
    void setFlightsData(QByteArray content);

};


#endif // FLIGHTSLISTMODEL_H
