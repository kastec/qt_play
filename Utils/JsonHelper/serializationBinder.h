
#ifndef SERIALIZATIONBINDER_H
#define SERIALIZATIONBINDER_H

#include <QObject>
#include <QMap>
#include <QMetaProperty>
#include <QJsonObject>

class SerializationBinder
{
    QString descriminator;
    QMap<QString, const QMetaObject*> mapper;
    
  public:
    SerializationBinder(){}
    SerializationBinder(QString descriminatorProp)
          :descriminator(descriminatorProp){};
    
    template <typename T>
    SerializationBinder& add(QString name){
        auto meta = &T::staticMetaObject;
        mapper[name] = meta;
        return *this;
    }
    
    const QMetaObject* getMetadata(QJsonObject &jobj) const {
        auto val = jobj.value(descriminator);
        if(val==QJsonValue::Undefined) return nullptr;
        
        auto key = val.toString();
        auto meta = mapper.value(key, nullptr);
        return meta;
    }
};

#endif // SERIALIZATIONBINDER_H
