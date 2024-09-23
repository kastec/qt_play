#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QObject>
#include <QBuffer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "qdebug.h"
//#include "nameOf.h"
#include "./JsonSerialization.h"

//#define SET_LIST_CHILDREN(propertyName, propertyListName, childType, jsonArrayData, parent, mapper)
//if (propertyName == QString(NAMEOF(propertyListName).data()))
//    return QVariant::fromValue(JsonSerialization::toArray<childType>(jsonArrayData, parent, mapper));


/// какие параметры подставлять - смотреть в  JsonSerialization::toArray<childType>(...)
/// Example: SET_LIST_CHILDREN(propname, "items", ClassToMap, json, parent, mapperFunc, &binder);
#define SET_LIST_CHILDREN(propertyName, propertyListName, childType, ...) \
if (propertyName == propertyListName) \
        return QVariant::fromValue(JsonSerialization::toArray<childType>(__VA_ARGS__));

/// Example: SET_LIST_CHILDREN_P(propname, [PARENT_CLASS_TYPE], "items", ItemA, json, parent, mapperFunc);
#define SET_LIST_CHILDREN_P(propertyName, parentType, propertyListName, childType, ...) \
if (propertyName == propertyListName) \
    if(dynamic_cast<parentType*>(parent) != nullptr) \
        return QVariant::fromValue(JsonSerialization::toArray<childType>(__VA_ARGS__));

class JsonHelper
{
    
  public:
    /*
       static void Test() {
   
         createListFunc flightMapper = [&](QString propname, QJsonArray json, QObject *parent) {
             SET_LIST_CHILDREN(propname, Flight::legs, FlightLeg, json, parent, flightMapper);
             SET_LIST_CHILDREN(propname, FlightTail::persons, FlightPerson, json, parent, flightMapper);
             return QVariant();
         };
 
          //=== 1 flight obj
          auto bytes = FileHelper::readAll("C:\\Projects\\QT_Test\\QtTest1\\flights-1.json");
          auto flightObj = JsonHelper::toObject<Flight>(bytes, nullptr, flightMapper);
  
         auto bFlight = JsonHelper::serialize(flightObj);
         FileHelper::save("C:\\Projects\\QT_Test\\QtTest1\\flights-1_SER.json", bFlight);
 
          //=== array flights
          auto bytesFlights = FileHelper::readAll("C:\\Projects\\QT_Test\\QtTest1\\flights-2.json");
          auto fls = JsonHelper::toArray<Flight>(bytesFlights, nullptr,flightMapper);
          auto leg1= fls[0]->legs[0];
          qDebug()<< (leg1->tail == nullptr);
          auto bytesArr = JsonHelper::serialize(fls);
          FileHelper::save("C:\\Projects\\QT_Test\\QtTest1\\flights-2_SER.json", bytesArr);
      }
      */
    
  public:
    template <class T>
    static T *toObject(QByteArray data, QObject* parent = nullptr,
                       const createListFunc &createList = nullptr,
                       const SerializationBinder *binder = nullptr) {
        QJsonObject jsonDoc = toObject(data);
        T *obj = JsonSerialization::toObject<T>(jsonDoc, parent, createList, binder);
        return obj;
    }
    
    static QJsonObject toObject(QByteArray data) {
        QJsonObject jsonObj;
        QJsonDocument jsonDoc = getJsonDocument(data);
        if (!jsonDoc.isNull())
            jsonObj = jsonDoc.object();
        return jsonObj;
    }
    
    template <class T>
    static QList<T *> toArray(QByteArray data, QObject* parent = nullptr,
                              const createListFunc &createList = nullptr,
                              const SerializationBinder *binder = nullptr)
    {
        auto jsonArray = toArray(data);
        QList<T*> array = JsonSerialization::toArray<T>(jsonArray, parent, createList, binder);
        return array;
    }
    
    static QJsonArray toArray(QByteArray data) {
        QJsonArray jsonArray;
        QJsonDocument jsonDoc = getJsonDocument(data);
        if (!jsonDoc.isNull())
            jsonArray = jsonDoc.array();
        return jsonArray;
    }
    
    static QByteArray toBytes(QJsonDocument jsonDoc) { return jsonDoc.toJson(QJsonDocument::Indented); }
    
    static QByteArray toBytes(QJsonArray obj) {
        QJsonDocument jsonDoc(obj);
        return toBytes(jsonDoc);
    }
    
    static QByteArray toBytes(QJsonObject obj) {
        QJsonDocument jsonDoc(obj);
        return toBytes(jsonDoc);
    }
    
  private:
    static QJsonDocument getJsonDocument(QByteArray data) {
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << data;
            qWarning() << "Json parse error: " << parseError.errorString();
        }
        return jsonDoc;
    }
    
  public:
    //  static QByteArray serialize(QObject obj) {
    //        auto jsonObj = JsonSerialization::serialize(&obj);
    //        auto bytes = toBytes(jsonObj);
    //        return bytes;
    //  }
    
    static QByteArray serialize(QObject *obj) {
        auto jsonObj = JsonSerialization::serialize(obj);
        auto bytes = toBytes(jsonObj);
        return bytes;
    }
    
    template <class T>
    static QByteArray serialize(QList<T*> list) {
        auto jsonArray = JsonSerialization::serialize(list);
        auto bytes = toBytes(jsonArray);
        return bytes;
    }
    
};

#endif // JSONHELPER_H
