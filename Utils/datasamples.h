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

template <typename T>
std::pair<QList<T>, QList<T>> split(QList<T> src, std::function<bool(const T&)> const pred) {
    QList<T> first;
    QList<T> second;
    for (const auto &v : src) {
        auto res = pred(v);
        if(res)
            first.append(v);
        else
            second.append(v);
    }  
    return {first, second};
}

#endif // DATASAMPLES_H
