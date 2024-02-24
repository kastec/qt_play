
#ifndef JSONLISTMODEL_H
#define JSONLISTMODEL_H


#include "qjsonarray.h"
#include <QObject>

class JsonListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonArray persons READ persons NOTIFY personsChanged)

public:
    explicit JsonListModel(QObject *parent = nullptr);

    QJsonArray persons();
    void setPersons(QJsonArray persons);

signals:
    void personsChanged();
private:
    QJsonArray m_persons;
};

#endif // JSONLISTMODEL_H


