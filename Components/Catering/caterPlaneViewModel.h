
#ifndef CATERPLANEVIEWMODEL_H
#define CATERPLANEVIEWMODEL_H

#include <QObject>
#include "Utils/JsonHelper/qqmlhelper.h"

#include "caterSeatViewModel.h"
#include "qsize.h"

class CaterPlaneViewModel : public QObject{
    Q_OBJECT
    
    QML_LIST_PROPERTY(QList<CaterSeatViewModel*>, items);
    
    
  public:
    
    Q_INVOKABLE explicit CaterPlaneViewModel(QObject *parent = nullptr);
    
    void loadLayout();
    
  private:
    QSize layoutSize;
};

#endif // CATERPLANEVIEWMODEL_H
