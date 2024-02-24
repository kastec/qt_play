#ifndef DATASAMPLES_H
#define DATASAMPLES_H

#include "Components/PersonList/person.h"
#include <QObject>
using namespace std;

class DataSamples
{
public:
    DataSamples();
    static QString GetColor();
    static QList<Person*> LoadJsonData();
    static QJsonArray LoadJsonArray();

private:
    static QList<QString> colors;
};

#endif // DATASAMPLES_H
