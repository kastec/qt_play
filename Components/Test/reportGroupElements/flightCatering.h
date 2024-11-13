
#ifndef FLIGHTCATERING_H
#define FLIGHTCATERING_H

#include <QObject>
#include <QDateTime>
#include "Utils/JsonHelper/qqmlhelper.h"
#include "Utils/JsonHelper/JsonHelper.h"
#include "Utils/JsonHelper/serializationBinder.h"
//#include "Utils/sharedQList.h"

class CateringClassService;

class MenuFoodCategory;
class MenuFoodGroup;
class MenuItem;

class Macronutrient;
class Recommended;
class MenuItemAddition;
class MenuItemOption;
class RationInfo;

class FlightCatering: public QObject{
    Q_OBJECT
    
    QML_READ_PROPERTY(QString, title)
    //    QML_READ_PROPERTY(QString, cksum)
    QML_READ_PROPERTY(CateringClassService*, business)
    QML_READ_PROPERTY(CateringClassService*, comfort)
    QML_READ_PROPERTY(CateringClassService*, economy)
    
  public:
    Q_INVOKABLE explicit FlightCatering(QObject *parent = nullptr): QObject {parent} {     }
    
    static FlightCatering* toObject(QByteArray data){
        createListFunc mapperFunc = [&](QString propname, QJsonArray json, QObject *parent) {
            SET_LIST_CHILDREN(propname, "rations", MenuFoodGroup, json, parent, mapperFunc);
            SET_LIST_CHILDREN(propname, "split", MenuFoodGroup, json, parent, mapperFunc);
            SET_LIST_CHILDREN(propname, "drinks", MenuFoodGroup, json, parent, mapperFunc);
            SET_LIST_CHILDREN(propname, "alcohol", MenuFoodGroup, json, parent, mapperFunc);
            SET_LIST_CHILDREN(propname, "cocktails", MenuFoodGroup, json, parent, mapperFunc);
            
            SET_LIST_CHILDREN(propname, "items", MenuItem, json, parent, mapperFunc);
            
            SET_LIST_CHILDREN(propname, "additions", MenuItemAddition, json, parent, mapperFunc);
            SET_LIST_CHILDREN(propname, "options", MenuItemOption, json, parent, mapperFunc);
            return QVariant();
        };
        
        auto catering = JsonHelper::toObject<FlightCatering>(data, nullptr, mapperFunc);
        return catering;
    };
};

class CateringClassService: public QObject{
    Q_OBJECT
    QML_READ_PROPERTY(MenuFoodCategory*, ru)
    QML_READ_PROPERTY(MenuFoodCategory*, en)
  public:
    Q_INVOKABLE explicit CateringClassService(QObject *parent = nullptr): QObject {parent} {     }
    
};

class MenuFoodCategory: public QObject{
    Q_OBJECT
    QML_LIST_PROPERTY(QList<MenuFoodGroup*>, rations)
    QML_LIST_PROPERTY(QList<MenuFoodGroup*>, split)
    QML_LIST_PROPERTY(QList<MenuFoodGroup*>, drinks)
    QML_LIST_PROPERTY(QList<MenuFoodGroup*>, alcohol)
    QML_LIST_PROPERTY(QList<MenuFoodGroup*>, cocktails)
    
  public:
    // список доступных рационов с кодами
    QStringList rationInfos;
    
           // все MenuItem'ы с ключем по коду (для быстрого получения)
    QMap<QString, MenuItem*> items;
    
           // группы из JSON сохранены в массив для удодсьва доступа и поиска (данные из rations, split, etc)
    QList<MenuFoodGroup*> foodGroups;
    
    Q_INVOKABLE explicit MenuFoodCategory(QObject *parent = nullptr): QObject {parent} {     }
    
    MenuItem* getItem(QString itemId)
    {
        
        auto item = items.value(itemId, nullptr);
        if(!item)
            qDebug()<<"can not find itemTitle with itemId:" << itemId;
        return item;
    }
    //    MenuFoodCategory() {
    //    }
};


class MenuFoodGroup: public QObject{
    Q_OBJECT
    QML_READ_PROPERTY(QString, ration)
    QML_READ_PROPERTY(QString, title)
    QML_READ_PROPERTY(int, selective)
    QML_LIST_PROPERTY(QList<MenuItem*>, items)
    
  public:
    Q_INVOKABLE explicit MenuFoodGroup(QObject *parent = nullptr): QObject {parent} {     }
    
    bool isLimited(){ return selective < 999;}
};

class MenuItem: public QObject{
    Q_OBJECT
    QML_READ_PROPERTY(QString, id)
    QML_READ_PROPERTY(QString, title)
    QML_READ_PROPERTY(QString, description)
    QML_READ_PROPERTY(QString, composition)
    QML_READ_PROPERTY(Macronutrient*, macronutrients)
    QML_READ_PROPERTY(QString, info)
    QML_READ_PROPERTY(QString, image)
    QML_LIST_PROPERTY(QList<MenuItemAddition*>, additions)
    QML_LIST_PROPERTY(QList<MenuItemOption*>, options)
    QML_READ_PROPERTY(bool, loaded)
    QML_READ_PROPERTY(Recommended*, recommended)
    QML_READ_PROPERTY(bool, limited)
    QML_READ_PROPERTY(QStringList, block)
    
  public:
    Q_INVOKABLE explicit MenuItem(QObject *parent = nullptr): QObject {parent} { }
    enum ItemCategory
    {
        Food,
        ColdDrink,
        HotDrink,
        Cocktail,
        Alcohol
    };
    
    ItemCategory category;
};

class Macronutrient: public QObject{
    Q_OBJECT
    QML_READ_PROPERTY(QString, proteins)
    QML_READ_PROPERTY(QString, fats)
    QML_READ_PROPERTY(QString, carbohydrates)
    QML_READ_PROPERTY(QString, calories)
    
  public:
    Q_INVOKABLE explicit Macronutrient(QObject *parent = nullptr): QObject {parent} {     }
    //    ~Macronutrients(){}
};

class MenuItemAddition: public QObject{
    Q_OBJECT
    QML_READ_PROPERTY(QString, name)
    QML_READ_PROPERTY(bool, separate)
    
  public:
    Q_INVOKABLE explicit MenuItemAddition(QObject *parent = nullptr): QObject {parent} {     }
    //    ~Additions(){}
};

class MenuItemOption:  public QObject{
    Q_OBJECT
    QML_READ_PROPERTY(QString, name)
    
  public:
    Q_INVOKABLE explicit MenuItemOption(QObject *parent = nullptr): QObject {parent} {     }
    //    ~MenuItemOption(){}
};

class Recommended: public QObject{
    Q_OBJECT
    QML_READ_PROPERTY(QString, international)
    QML_READ_PROPERTY(QString, internal)
  public:
    Q_INVOKABLE explicit Recommended(QObject *parent = nullptr): QObject {parent} {     }
    //    ~Recommended(){}
};
#endif // FLIGHTCATERING_H
