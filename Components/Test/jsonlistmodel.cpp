#include "Utils/datasamples.h"
#include "jsonlistmodel.h"

JsonListModel::JsonListModel(QObject *parent)
    : QObject{parent}
{
    auto persons = DataSamples::LoadJsonArray();
    m_persons = persons;
}

QJsonArray JsonListModel::persons()
{
    return m_persons;
}

void JsonListModel::setPersons(QJsonArray persons)
{
    m_persons = persons;
    emit personsChanged();
}
