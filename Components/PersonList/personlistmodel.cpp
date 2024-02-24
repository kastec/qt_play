#include "personlistmodel.h"
#include "Components/PersonList/person.h"
#include "qdebug.h"
#include "Utils/datasamples.h"
#include "qqmllist.h"



PersonListModel::PersonListModel(QObject *parent)
    : QObject{parent}
{

    auto persons = DataSamples::LoadJsonData();
    m_persons = persons;
    
  /*
   
    auto p= new Person();
    p->age=15;
     p->name="1515 qwertyu";
    //QString qstr = "1515";
    //p->setProperty("name", "1515");
     m_persons.append(p);

    auto p2= new Person();
    p2->age=40;
    p2->name="404040";
    m_persons.append(p2);

    auto p3= new Person();
    p3->age=25;
    p3->name="25 25";
    m_persons.append(p3);
 */
    //qDebug() << "persons: " << m_persons.count();
}

Person* PersonListModel::getPerson(int index)
{
    auto p = m_persons[index];
  //  qDebug() << "get " << index << ":" << p->name;
    return p;
}

void PersonListModel::log(int index)
{
    auto p = m_persons[index];
    qDebug() << "["<<index<<"]:" << p->name << p->age;
}

QList<Person*> PersonListModel::persons()
{
    return m_persons;
}

void PersonListModel::setPersons(QList<Person*> persons)
{
    m_persons = persons;
    emit personsChanged();
}
//void PersonListModel::setPersons(QList<Person*> persons)
//{
//    m_persons = persons;
//    emit personsChanged();
//}

