
#ifndef DERIVEDDESERIALIZETEST_H
#define DERIVEDDESERIALIZETEST_H

#include <QObject>
#include "Utils/JsonHelper/FileHelper.h"

#include "Utils/JsonHelper/qqmlhelper.h"
#include "Utils/JsonHelper/JsonHelper.h"
#include "Utils/JsonHelper/JsonSerialization.h"
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
            
//            if (propertyName == propertyListName) \
//                    return QVariant::fromValue(JsonSerialization::toArray<childType>(__VA_ARGS__));
            
            
            return QVariant();
        };
        
        auto flightData = JsonHelper::toObject<PersonsReportList>(data, nullptr, mapperFunc);
        
        return flightData;
    }
};

/*
class Base {
  public:
    void print()
    {
        qDebug()<< "Base" ;
    }
    virtual  ~Base(){};
};

// Derived Class 1 declaration
class Derived1 : public Base {
    void print()
    {
        qDebug() << "Derived1" ;
    }
//     virtual  ~Derived1();
};

// Derived class 2 declaration
class Derived2 : public Base {
    void print()
    {
        qDebug() <<"Derived2";
    }
};
*/

class ItemA : public  QObject{
    Q_OBJECT
    QML_WRITABLE_PROPERTY(QString, aName);
    
  public:
    Q_INVOKABLE explicit ItemA(QObject *parent = nullptr): QObject {parent} {     }
};


class ItemB : public  QObject{
    Q_OBJECT
    QML_WRITABLE_PROPERTY(QString, bName);
    
  public:
    Q_INVOKABLE explicit ItemB(QObject *parent = nullptr): QObject {parent} {     }
};


class AGroups : public  QObject{
    Q_OBJECT
    
    QML_LIST_PROPERTY(QList<ItemA*>, items);
    
  public:
    Q_INVOKABLE explicit AGroups(QObject *parent = nullptr): QObject {parent} {     }
};

class BGroups : public  QObject{
    Q_OBJECT
    
    QML_LIST_PROPERTY(QList<ItemB*>, items);
    
  public:
    Q_INVOKABLE explicit BGroups(QObject *parent = nullptr): QObject {parent} {     }
};

class AllGroups : public  QObject{
    Q_OBJECT
    
    QML_WRITABLE_PROPERTY(AGroups*, aGroup) 
    QML_WRITABLE_PROPERTY(BGroups*, bGroup)
    
  public:
    Q_INVOKABLE explicit AllGroups(QObject *parent = nullptr): QObject {parent} {     }
    
  public:
    static AllGroups* toObject(QByteArray data)
    {
        
        createListFunc mapperFunc = [&](QString propname, QJsonArray json, QObject *parent){
            // SET_LIST_CHILDREN(propname, "items", ItemA, json, parent, mapperFunc, nullptr);

            SET_LIST_CHILDREN_P(propname, AGroups, "items", ItemA, json, parent, mapperFunc);
            SET_LIST_CHILDREN_P(propname, BGroups, "items", ItemB, json, parent, mapperFunc);
/*            
            if (propname == "items") {
                AGroups* parentTyped = dynamic_cast<AGroups*>(parent);
                if(parentTyped!=nullptr)
                    return QVariant::fromValue(JsonSerialization::toArray<ItemA>(json, parent, mapperFunc, nullptr));
            }
            if (propname == "items") {
                BGroups* parentTyped = dynamic_cast<BGroups*>(parent);
                if(parentTyped!=nullptr)
                    return QVariant::fromValue(JsonSerialization::toArray<ItemB>(json, parent, mapperFunc, nullptr));
            }
            //            if (propertyName == propertyListName) \
            //                    return QVariant::fromValue(JsonSerialization::toArray<childType>(__VA_ARGS__));
            
  */          
            return QVariant();
        };
        
        auto flightData = JsonHelper::toObject<AllGroups>(data, nullptr, mapperFunc);
        
        return flightData;
    }
};

class DeserializeTest
{
  public:
    void Test(){
        qDebug()<< "--- START ----------------------";
        auto data = FileHelper::readAll("D:\\0\\_serGroups.json");
        auto res = AllGroups::toObject(data);
        qDebug()<< "res: " << res;
        qDebug()<< "res->aGroup->items: " << res->aGroup->items.length();
        auto ia = res->aGroup->items[0];
        qDebug()<< "item-aname: " << ia->aName;
        
        qDebug()<< "res->bGroup->items: " << res->bGroup->items.length();
        auto ib = res->bGroup->items[0];
        qDebug()<< "item-bname: " << ib->bName;
        
        auto d= JsonHelper::serialize(res);
        FileHelper::save("D:\\0\\_serGroups_out.json",d);
        
        qDebug()<< "-------------------------";
//        Derived1 d1;        
//        Base* bp = dynamic_cast<Base*>(&d1);

//        Derived1* dp2 = dynamic_cast<Derived1*>(bp);
//        qDebug()<< "dyn_cast: " << (dp2?"YES":"NULL");
    }
};
#endif // DERIVEDDESERIALIZETEST_H






