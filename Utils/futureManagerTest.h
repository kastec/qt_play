
#ifndef FUTUREMANAGERTEST_H
#define FUTUREMANAGERTEST_H

#include "Utils/HttpRequester/httpRequester.h"
#include "futureManager.h"

class FutureManagerTest
{
  public:
    
    void startTest3(){
        FutureManager man;
        auto res =  man.addTask([this]() -> bool {return checkGroundConnection();})
                       .addTask([this]() -> bool {QThread::sleep(2);return checkGroundConnection();})
                       .waitAll();
        qDebug() << "  ---:wait";
        bool isGroundConn = res->getResult<bool>(0);
        qDebug() << "  ---:wait-1";
        bool isOnBoardWifi = res->getResult<bool>(1);
        qDebug() << "  ---:"<<isGroundConn<<isOnBoardWifi;
    }
    
    bool checkGroundConnection()
    {        
        HttpRequester requester(nullptr);         
        requester.initRequester("https://kontron-box.pifet.ru/api/board/aircraftNumber");
        QByteArray data = requester.sendRequestWait("");
        auto json = QString(data);
        qDebug()<< "  loaded " <<  json;
        return true;
    }
    
    void startTest2()
    {
        FutureManager manager;
        manager.addTask([]() -> int {
            qDebug() << "  --start 2";
            QThread::sleep(2);
            return 42;
        })
        .addTask([]() -> QString {
            qDebug() << "  --start json";
            QThread::sleep(3);
            return "{Some Json}";
        });
        auto r = manager.waitAll();
        qDebug() << "results:" << r->getResult<int>(0) << r->getResult<QString>(1);
    }
    void startTest()
    {        
        auto manager = new FutureManager();
        
        // Добавляем задачи
        manager->addTask([]() -> int {
            qDebug() << "  --start 2";
            QThread::sleep(2);
            return 42;
        });
    
        manager->addTask([]() -> QString {
            qDebug() << "  --start hello";
            QThread::sleep(1);
            return "Hello";
        });
        
        manager->addTask([]() -> QString {
            qDebug() << "  --start json";
            QThread::sleep(3);
            return "{Some Json}";
        });
        
        manager->addTask([]() -> QString {
            qDebug() << "  --start HttpReq";
            QThread::sleep(2);
            //return "{---}";
            HttpRequester requester(nullptr);         
            requester.initRequester("https://kontron-box.pifet.ru/api/board/aircraftNumber");
            QByteArray data = requester.sendRequestWait("");
            auto json = QString(data);
            //qDebug()<< "  loaded " <<  json;
            return json;
        });
        
        /*
        // 1. Вызов через callback        
        manager->setCallback([=](FutureResults* results) {           
            showResults(results);            
            manager->deleteLater();
        });
*/
        
    /*
        // 2. Вызов через сигнал
        FutureManager::connect(manager,&FutureManager::allCompleted,
                               [=](FutureResults* results) {
                                   showResults(results);
                                   manager->deleteLater();
                               } );        
    */
        
     // 3. Вызов с локом потока
       auto results = manager->waitAll();
       showResults(results);
       delete manager;
 //      manager->deleteLater();
       
    }
    
  private:
    void showResults(FutureResults* results){
        try {
            
            int first = results->getResult<int>(0);
            QString second = results->getResult<QString>(1);
            QString json3 = results->getResult<QString>(2);
            QString http = results->getResult<QString>(3);
           
            qDebug() << "Results CALLBACK:" << first << second << json3 << http;
            //          qDebug() << "Results:  - fin" ;
        } catch (const std::exception& e) {
            qDebug() << "Error:" << e.what();
        }         
    }
};

#endif // FUTUREMANAGERTEST_H
