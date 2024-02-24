
#ifndef FLIGHT_H
#define FLIGHT_H


#include "Utils/JsonHelper/qqmlhelper.h"
#include "qdatetime.h"
#include <QObject>



class FlightLeg;
class FlightTail;
class FlightPerson;
    
class Flight: public QObject
{    
    Q_OBJECT
    
    QML_READ_PROPERTY(int, id);
    QML_WRITABLE_PROPERTY(int, number);
    QML_READ_PROPERTY(QDateTime, date);
    
    QML_WRITABLE_PROPERTY(QStringList, items);
        
    QML_READ_PROPERTY(QDateTime, dateLocal);
    
    QML_READ_PROPERTY(QList<FlightLeg *>, legs);
    
    QML_READ_PROPERTY(QString, originAirportCode);
    QML_READ_PROPERTY(QString, originCity);
    
    QML_READ_PROPERTY(QString, destAirportCode);
    QML_READ_PROPERTY(QString, destCity);
    QML_WRITABLE_PROPERTY(int, crewCount);
    QML_WRITABLE_PROPERTY(int, passengersCount);
    
        
  public:
    
    Q_INVOKABLE explicit Flight(): QObject{}
    {
        
    }
    
   // explicit Flight(QObject *parent = nullptr);
//   Q_INVOKABLE explicit Flight(QObject *parent): QObject{parent}
//    {
        
//    }

   Q_INVOKABLE void info() {
//        qDebug() << "Flight (Id:" << id << "number:" << number << "city:" << originCity << "date: " << date << ")";
       qDebug() << "Flight (Id:" << id ;
   }
};


Q_DECLARE_METATYPE(Flight)


class FlightLeg: public QObject
{    
   Q_OBJECT
   QML_READ_PROPERTY(int, passCount);
   QML_READ_PROPERTY(QDateTime, date);
   QML_READ_PROPERTY(QString, gate);
   
   QML_WRITABLE_PROPERTY(FlightTail*, tail);
   
 public:
   
   Q_INVOKABLE explicit FlightLeg(): QObject{}
   {
        
   }
   
   // explicit Flight(QObject *parent = nullptr);
//   Q_INVOKABLE explicit FlightLeg(QObject *parent): QObject{parent}
//   {
        
//   }
   
   Q_INVOKABLE void info() {
        qDebug() << "FlightLeg (gate:" << gate << "passCount:" << passCount<<")";
   }
};


//Q_DECLARE_METATYPE(FlightLeg)
//Q_DECLARE_METATYPE(FlightLeg*)
//Q_DECLARE_METATYPE(QList<FlightLeg*>)
//Q_DECLARE_METATYPE(QList<FlightLeg*>*)

class FlightTail: public QObject
{    
   Q_OBJECT
   QML_READ_PROPERTY(int, number);
   QML_READ_PROPERTY(QString, name);

   QML_READ_PROPERTY(QList<FlightPerson *>, persons);

 public:
   Q_INVOKABLE explicit FlightTail(): QObject{}
   {
   }
   
//   Q_INVOKABLE FlightTail(QObject *parent): QObject{parent}
//   {        
//   }
  
   
};

//Q_DECLARE_METATYPE(FlightTail)
//Q_DECLARE_METATYPE(FlightTail*)


class FlightPerson: public QObject
{    
   Q_OBJECT
   QML_READ_PROPERTY(int, id);
   QML_READ_PROPERTY(int, tabelNumber);
   QML_READ_PROPERTY(QString, name);
   
 public:
   
   Q_INVOKABLE explicit FlightPerson(): QObject{}
   {        
   }
   
//   Q_INVOKABLE FlightPerson(QObject *parent): QObject{parent}
//   {        
//   }
//   ~FlightPerson()
//   {
//        qDebug()<<"destr person:" << name;
//   }
};

//Q_DECLARE_METATYPE(FlightPerson)
//Q_DECLARE_METATYPE(FlightPerson*)



#endif // FLIGHT_H
