#include "datasamples.h"
#include <QObject>
#include <random>
#include <QFile>
#include <QDir>
#include "Utils/JsonHelper/JsonSerialization.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
#include <QJsonDocument>
#include "Components/PersonList/person.h"
#include "HttpRequester/httpRequester.h"

using namespace std;


QList<QString> DataSamples::colors = {"red", "skyblue", "green", "blue",
                                      "gray","cyan", "yellow","lime", "fuchsia","coral","chocolate",
                                      "darkred","deeppink", "pink"};

DataSamples::DataSamples()
{
    //DataSamples::colors.append("red");
   // colors.append("red");
}

QString DataSamples::GetColor() {
    auto i = rand() % colors.count();
    auto str = colors[i];
    return str;
}

QJsonArray DataSamples::LoadJsonArray()
{
     QFile file("C:\\Projects\\QT_Test\\QtTest1\\data.json");
    file.open(QIODevice::ReadOnly);
    QByteArray rawData = file.readAll();
    file.close();
    QJsonDocument doc(QJsonDocument::fromJson(rawData));
    QJsonArray items = doc.array();
    
    return items;
}


QList<Person*> DataSamples::LoadJsonData()
{
  QFile file("C:\\Projects\\QT_Test\\QtTest1\\data.json");
  file.open(QIODevice::ReadOnly);
  QByteArray rawData = file.readAll();
  file.close();
  QJsonDocument doc(QJsonDocument::fromJson(rawData));
  QJsonArray items = doc.array();

  QList<Person*> personList;

  for(auto jsonObj  : items)   {
      QJsonObject pers = jsonObj.toObject();
      auto  name = pers["name"].toString();
      //qDebug() << "name:" << name;
      auto  age = pers["age"].toInt();
     // qDebug() << "age:" << age;

      auto person= new Person();

      person->name = name;
      person->age=age;
      person->seat= pers["seat"].toString();
      person->enabled= pers["enabled"].toBool();
      personList.append(person);
  }

  return personList;
}




//void DataSamples::SaveFile(QString file, QJ){  }
