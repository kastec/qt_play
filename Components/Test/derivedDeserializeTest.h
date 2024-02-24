
#ifndef DERIVEDDESERIALIZETEST_H
#define DERIVEDDESERIALIZETEST_H

#include <QObject>
#include "Utils/JsonHelper/qqmlhelper.h"
#include "Utils/JsonHelper/JsonHelper.h"
#include "Utils/JsonHelper/serializationBinder.h"

class ReportItemBase : public  QObject{
    Q_OBJECT
    
    QML_WRITABLE_PROPERTY(QString, typeObj);
    QML_WRITABLE_PROPERTY(int, position);
    QML_WRITABLE_PROPERTY(QString, someText);
    
    public:
    Q_INVOKABLE explicit ReportItemBase(QObject *parent = nullptr): QObject {parent} {     }
};

class PersonReportItem: public ReportItemBase{    
    Q_OBJECT
    
    QML_WRITABLE_PROPERTY(int, age);
    QML_WRITABLE_PROPERTY(QString, firstName);
    
    public:
    Q_INVOKABLE explicit PersonReportItem(QObject *parent = nullptr): ReportItemBase(parent) {     }
};

class AddressReportItem: public ReportItemBase{
    Q_OBJECT
    
    QML_WRITABLE_PROPERTY(QString, city);
    QML_WRITABLE_PROPERTY(int, house);
    QML_WRITABLE_PROPERTY(int, flat);    
    
  public:
    Q_INVOKABLE explicit AddressReportItem(QObject *parent = nullptr):ReportItemBase(parent) {     }
};

class AddressReportExItem: public AddressReportItem{
    Q_OBJECT
    
    QML_WRITABLE_PROPERTY(QString, phone);
    QML_WRITABLE_PROPERTY(int, index);
    QML_WRITABLE_PROPERTY(QStringList, cars);
    
  public:
    Q_INVOKABLE explicit AddressReportExItem(QObject *parent = nullptr):AddressReportItem(parent) {     }
};

class PersonsReportList : public QObject{
    Q_OBJECT
    QML_WRITABLE_PROPERTY(int, testval);    
    QML_LIST_PROPERTY(QList<ReportItemBase*>, items);
    
  public:
    Q_INVOKABLE explicit PersonsReportList(QObject *parent = nullptr): QObject {parent} {     }
    
  public:
    static PersonsReportList* toObject(QByteArray data)
    {
        auto binder = SerializationBinder("typeObj")
                          .add<PersonReportItem>("personType")
                          .add<AddressReportItem>("addressType")
                          .add<AddressReportExItem>("addressExtendedType");
        
        
        createListFunc mapperFunc = [&](QString propname, QJsonArray json, QObject *parent){
            SET_LIST_CHILDREN(propname, "items", ReportItemBase, json, parent, nullptr, &binder);
            return QVariant();
        };
        
        auto flightData = JsonHelper::toObject<PersonsReportList>(data, nullptr, mapperFunc);
        
        return flightData;
    }
};

#endif // DERIVEDDESERIALIZETEST_H






