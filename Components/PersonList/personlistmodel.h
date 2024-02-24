#ifndef PERSONLISTMODEL_H
#define PERSONLISTMODEL_H

#include <QObject>
#include "qqmllist.h"

class Person;

class PersonListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<Person*> persons READ persons NOTIFY personsChanged)
   
    Q_CLASSINFO("DefaultProperty", "persons")
public:
    explicit PersonListModel(QObject *parent = nullptr);    

    QList<Person*> persons();
    void setPersons(QList<Person*> persons);
 
     Q_INVOKABLE Person* getPerson(int index);

     Q_INVOKABLE void log(int index);
signals:
    void personsChanged();

private:
    QList<Person*> m_persons;
};

#endif // PERSONLISTMODEL_H
