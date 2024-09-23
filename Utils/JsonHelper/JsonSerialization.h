
#ifndef JSONSERIALIZATION_H
#define JSONSERIALIZATION_H

#include <QJsonObject>
#include <QMetaProperty>
#include <QJsonDocument>

#include <functional>
#include <QJsonObject>
#include <QJsonArray>

#include "qdebug.h"
#include "qsequentialiterable.h"
#include "serializationBinder.h"
//#include "../logger/logger.h"

//#include "FileHelper.h" // test
//#include "JsonHelper.h"
//#include "../nameoff.h"
//#include "qsequentialiterable.h"



typedef std::function<QVariant(QString name, QJsonArray jsArray, QObject *parent)> createListFunc;

class JsonSerialization
{
    
  public:
    template <class T>
    
    static T* toObject(QJsonObject jsonObject, QObject* parent = nullptr,
                       const createListFunc &createList = nullptr,
                       const SerializationBinder *binder = nullptr) {
        T* res = (T*) toObject(&T::staticMetaObject, jsonObject, parent, createList, binder);
        return res;
    }
    
    template <class T>
    static QList<T *> toArray(QJsonArray &arr, QObject* parent = nullptr,
                              const createListFunc &createList = nullptr,
                              const SerializationBinder *binder = nullptr)
    {
        QList<T *> qlist;
        for (auto v : arr) {
            auto jsItem = v.toObject();
            T *item = (T *)toObject(T::staticMetaObject, jsItem, parent, createList, binder);
            //            item->setParent(parent);
            qlist.append(item);
        }
        return qlist;
    }
    
  private:
    static QObject *toObject(const QMetaObject &meta, QJsonObject jsonObject, QObject* parent,
                             const createListFunc &createList,
                             const SerializationBinder *binder) {
        return toObject(&meta, jsonObject, parent, createList, binder);
    }
    
    static QObject *toObject(const QMetaObject *meta, QJsonObject &jsonObject, QObject *parent,
                             const createListFunc &createList,
                             const SerializationBinder *binder) {
        
        const QMetaObject * objMetadata = (binder!=nullptr)? binder->getMetadata(jsonObject): nullptr;
        objMetadata = objMetadata!=nullptr?objMetadata: meta;
        
        QObject *object = objMetadata->newInstance();
        if(object==nullptr)
        {
            qDebug()<<QString("ERROR METADATA: CAN'T CREATE object %1").arg(meta->className());
//            Logger::logError(QString("ERROR METADATA: CAN'T CREATE object %1").arg(meta->className()));
        }
        
        if (parent != nullptr)
            object->setParent(parent);
        
        int propertyStart = QObject::staticMetaObject.propertyCount();
        for (int i = propertyStart; i < objMetadata->propertyCount(); ++i) {
            QMetaProperty property = objMetadata->property(i);
            
            if (!jsonObject.contains(property.name())){
                // set default value
                property.write(object, QVariant());
                continue;
            }
            
            auto value = jsonValueToProperty(object, property, jsonObject.value(property.name()), createList, binder);
            property.write(object, value);
        }
        return object;
    }
    
    static QVariant jsonValueToProperty(QObject *object, QMetaProperty &property, QJsonValue value,
                                        const createListFunc &createList,
                                        const SerializationBinder *binder
                                        ) {
        auto type = property.userType();
        auto propname = property.name();
        //        auto propTypeName = property.typeName();
        //        auto typeName = QString(property.typeName());
        
               //        if (propname == QString("tail")) { // test prop for debug
               //            qDebug() << "prop: " << propname;
               //        }
        
        if (value.isArray()) {
            if(type==QMetaType::QStringList)
            {
                auto sl = value.toVariant().toStringList();
                return QVariant::fromValue(sl);
            }
            
            if (createList != nullptr) {
                auto propValue = createList(propname, value.toArray(), object);
                return propValue;
            } else
                return QVariant();
            
        } else if (value.isObject()) {
            switch (type) {
                //specific cases
            default:
                auto jsonObject = value.toObject();
                auto qQbj = toObject(QMetaType::metaObjectForType(type), jsonObject, object, createList, binder);
                return QVariant::fromValue(qQbj);
                break;
            }
            //todo other meta object types
        } else {
            //primitive types
            return value.toVariant();
        }
        
        return "";
    }
    
  public:
    template <class  T>
    static const QJsonArray serialize(QList<T*> &list){
        QJsonArray jsonArray;
        for (T* v : list) {
            auto subobjJson = serialize(v);
            jsonArray.append(subobjJson);
        }
        return jsonArray;
    }
    
    static const QJsonObject serialize(QObject *object)
    {
        QJsonObject jobj;
        
        auto meta = object->metaObject();
        QList<QString> shouldNotDeserializeList;
        int propertyStart = QObject::staticMetaObject.propertyCount();
        // цикл составления списка полей
        // которые не будут сериализованы (префикс ShouldSerialize)
        for (int i = propertyStart; i < meta->propertyCount(); ++i) {
            QMetaProperty prop = meta->property(i);
            
            QString propname = prop.name();
            //поля shouldSerialize должны быть bool по типу
            if(propname.startsWith("ShouldSerialize"))
            {
                if(std::string(prop.typeName()) == "bool" )
                {
                    bool propertyValue = prop.read(object).value<bool>();
                    if(propertyValue == false)
                    {
                        QString serializableProperty = propname.remove(0,15);
                        shouldNotDeserializeList.append(serializableProperty);
                    }
                }
            }
        }
        
               //цикл сериализации
        for (int i = propertyStart; i < meta->propertyCount(); ++i) {
            QMetaProperty prop = meta->property(i);
            
            auto proptype = prop.userType();
            auto propname = prop.name();
            
            if (QString(propname).startsWith("ShouldSerialize"))
            {
                //Logger::logInfo((QString("should not serialize was found ") + QString(propname)));
                continue;
            }
            
            if (shouldNotDeserializeList.contains(propname))
            {
                //Logger::logInfo((QString("should not serialize was found ") + QString(propname)));
                continue;
            }
            QVariant variant = object->property(propname);
            
                   // test prop for debug
                   //            if(propname == QString("tail")){
                   //                qDebug() << "prop: " << propname;
                   //                qDebug() << "isValid: " << variant.isValid();
                   //                qDebug() << "isNull: " << variant.isNull();
                   //            } //QString sd;
                   //bool ShouldSerializeSd;
                   //можно по массиву бегать искать ShouldSerializeValue
                   //по нему ориентироваться нужно сериализовать или нет
                   //а после выкинуть эти поля
            
            if(proptype == QMetaType::QString)
            { // обработаем строку здесь, чтобы она не мешалась далее обработке массивов
                jobj.insert(propname, QJsonValue::fromVariant(variant));
                continue;
            }
            
            if (variant.canConvert<QObject *>()) {
                // объект
                if (!variant.isValid() || variant.isNull()){ // может быть что объект не задан (не валиден) -> null
                    jobj.insert(propname, QJsonValue());
                    continue;
                }
                
                QObject* sub_obj = variant.value<QObject*>();
                if(sub_obj==nullptr) { // почему-то иногда проверка выше variant.isNull() не проходит, прходится костылять
                    jobj.insert(propname, QJsonValue());
                    continue;
                };
                
                auto jsonvalue = serialize(sub_obj);
                jobj.insert( propname, jsonvalue);
                
                continue;
            }
            
            if (variant.canConvert<QStringList>())
            {
                QStringList sl = variant.value<QStringList>();
                QJsonArray jsonArray;
                for (const QString &str : sl) {
                    jsonArray.append(QJsonValue::fromVariant(str));
                }
                jobj.insert(propname,  jsonArray);
                continue;
            }
            
                   // массив объектов
            if (variant.canConvert<QVariantList>())
                                                    //                       &&!variant.canConvert<QString>() // строка - это List из char - сюда попадать нельзя
            {
                // для бинда на экран для массивов мы созвавали для QML специальное свойство [propName]Display
                // поэтому его надо пропустить
                if(QString(propname).endsWith("Display"))
                    continue;
                
                QJsonArray jsonArray;
                auto iterable = variant.value<QSequentialIterable>();
                
                for (const QVariant &v : iterable) {
                    auto subobj = v.value<QObject *>();
                    auto subobjJson = serialize(subobj);
                    jsonArray.append(subobjJson);
                }
                jobj.insert(propname, jsonArray);
                continue;
            }
            
                   // примитивы/*
            jobj.insert(propname, QJsonValue::fromVariant(variant));
        }
        
        return jobj;
    }
    
};

#endif // JSONSERIALIZATION_H

