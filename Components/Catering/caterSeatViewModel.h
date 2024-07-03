
#ifndef CATERSEATVIEWMODEL_H
#define CATERSEATVIEWMODEL_H

#include <QObject>
#include "Utils/JsonHelper/qqmlhelper.h"


class CaterSeatViewModel : public QObject{
    Q_OBJECT
    
    QML_WRITABLE_PROPERTY(int, x);
    QML_WRITABLE_PROPERTY(int, y);
    QML_READ_PROPERTY(QString, name);
    
  public:
    
    Q_INVOKABLE explicit CaterSeatViewModel(QObject *parent = nullptr): QObject{parent}
    {        
    }
    
};

#endif // CATERSEATVIEWMODEL_H
