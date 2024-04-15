
#ifndef APPMESSAGEBUS_H
#define APPMESSAGEBUS_H

#include "Utils/JsonHelper/qqmlhelper.h"
//#include "qjsengine.h"
//#include "qqmlengine.h"

#include <QObject>

class AppMessageBus: public QObject
{
    Q_OBJECT
    SINGLETON_PATTERN_DECLARE(AppMessageBus)
    
  public:
    AppMessageBus(QObject *parent = nullptr);
    
  signals:
    void logEvent(const QString& str);
    void createBook(const QString& author);
    void inputChanged(QString inputId, bool on);
   
};

#endif // APPMESSAGEBUS_H
