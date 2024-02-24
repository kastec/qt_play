#include <QObject>
#include <QFile>
#include <QDir>

#include <QJsonDocument>
#include "Utils/HttpRequester/httpRequester.h"
#include "qdebug.h"
#include "Utils/JsonHelper/FileHelper.h"
#include "Utils/JsonHelper/JsonHelper.h"

#include "flightslistmodel.h"

FlightsListModel::FlightsListModel(QObject *parent) : QObject{parent}
{
}

void FlightsListModel::loadJsonArray() { loadJsonArrayUrl("/api/flights/6/2023-09-09"); }
void FlightsListModel::loadJsonArray2() { loadJsonArrayUrl("/api/flights/6/2023-09-12"); }

void FlightsListModel::loadJsonArrayUrl(QString apiUrl)
{
    qDebug() << "call FlightsListModel";
         
    auto *requester = new HttpRequester(nullptr);
       
    requester->initRequester("https://cms.ife.one:443");
    requester->sendRequest(
        apiUrl,
        [this](QByteArray data) mutable { this->setFlightsData(data); },
        [](QByteArray res) { qDebug() << "error callback"; }, HttpRequester::Type::GET);

    qDebug() << "exit Load func";  
}

void FlightsListModel::setFlightsData(QByteArray content) {
    qDebug() << "successt callback";
    FileHelper::save("C:\\Projects\\QT_Test\\QtTest1\\flights.json", content);
    
//    createListFunc flightMapper = [&](QString propname, QJsonArray json, QObject *parent) {
//        SET_LIST_CHILDREN(propname, Flight::legs, FlightLeg, json, parent, flightMapper);
//        SET_LIST_CHILDREN(propname, FlightTail::persons, FlightPerson, json, parent, flightMapper);
//        return QVariant();
//    };
    
    createListFunc flightMapper = [&](QString propname, QJsonArray json, QObject *parent) {
        SET_LIST_CHILDREN(propname, "legs", FlightLeg, json, parent, flightMapper);
        SET_LIST_CHILDREN(propname, "persons", FlightPerson, json, parent, flightMapper);
        return QVariant();
    };
    
    // Set JsonArray Flights
    auto jsonFlights = JsonHelper::toArray(content);
    this->set_jsFligths(jsonFlights);

    // Set Flight-object list
    auto flights = JsonHelper::toArray<Flight>(content, this);
    this->set_fligths(flights);
    
}

void FlightsListModel::saveFlights() {
    auto data = JsonHelper::serialize(this->fligths);
    FileHelper::save("C:\\Projects\\QT_Test\\QtTest1\\flights-serialize.json", data);
}




