
#ifndef APPMESSAGEBUS_H
#define APPMESSAGEBUS_H

#include "Utils/JsonHelper/qqmlhelper.h"
#include "qjsengine.h"
#include "qqmlengine.h"

#include <QObject>

class AppMessageBus: public QObject
{
    Q_OBJECT
    SINGLETON_PATTERN_DECLARE(AppMessageBus)
    
  public:
    AppMessageBus(QObject *parent = nullptr);
    
    static QObject* qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);
        
        auto inst = AppMessageBus::i();
        return inst;
    }
  signals:
    void logEvent(const QString& str);
    void createBook(const QString& author);
    void inputChanged(QString inputId, bool on);
   
};

#endif // APPMESSAGEBUS_H
