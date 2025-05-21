
#ifndef FUTUREMANAGER2TEST_H
#define FUTUREMANAGER2TEST_H

#include "Utils/HttpRequester/httpRequester.h"
#include "futureManager2.h"

class FutureManager2Test
{
    
  public:
    void startTest2()
    {
       auto [i,s] = FutureManager2<int, QString>().startTasks(
            [](){
                qDebug() << "  --start 2";
                QThread::sleep(3);
                qDebug() << "  --done  2";
                return 42;
            },
            [](){
                qDebug() << "  --start HttpReq";
                QThread::sleep(2);
                //return "{---}";
                HttpRequester requester(nullptr);         
                requester.initRequester("https://kontron-box.pifet.ru/api/board/aircraftNumber");
                QByteArray data = requester.sendRequestWait("");
                auto json = QString(data);
                qDebug() << "  --doen  HttpReq";
                return json; 
            }
            );
        qDebug() << "results:" <<i << s;       
    }
    
};

#endif // FUTUREMANAGER2TEST_H
