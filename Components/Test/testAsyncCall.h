
#ifndef TESTASYNCCALL_H
#define TESTASYNCCALL_H

#include <functional>
#include <future>
#include "Utils/HttpRequester/httpRequester.h"
#include "qeventloop.h"
#include "qfuture.h"
#include <QObject>
#include <QFutureWatcher>
#include <thread>
#include <QtConcurrent/QtConcurrentRun>

#include "QDebug"


template <typename T>
using Func = std::function<T(T)>;


// https://stackoverflow.com/questions/73035602/how-do-you-wait-for-qtconcurrentrun-to-finish-without-thread-blocking
//https://stackoverflow.com/questions/71862558/ui-blocks-while-the-program-is-running-in-qt-c
//template <class T>
//auto asynchronous( auto &&lambda )
//{
//    QEventLoop wait;
//    QFutureWatcher<T> fw;
//    fw.setFuture( QtConcurrent::run(lambda) );
//    QObject::connect   ( &fw, &QFutureWatcher<T>::finished, &wait, &QEventLoop::quit );
//    wait.exec();
//    QObject::disconnect( &fw, &QFutureWatcher<T>::finished, &wait, &QEventLoop::quit );
//    return fw.result();
//}

//https://stackoverflow.com/questions/17418334/variadic-template-to-proxy-qtconcurrentrun-functions


template <class F, class... Args>
static auto Run(F&& f, Args&&... args) 
    -> QFuture<decltype(f(std::forward<Args>(args)...))>
{
    QEventLoop wait;
    
    QFutureWatcher<void> futureWatcher; 
    auto future = QtConcurrent::run(std::forward<F>(f), std::forward<Args>(args)...);
    futureWatcher.setFuture(future);
    
    QObject::connect(&futureWatcher, &QFutureWatcher<F>::finished, &wait, &QEventLoop::quit);
    wait.exec();
    QObject::disconnect(&futureWatcher, &QFutureWatcher<F>::finished, &wait, &QEventLoop::quit );
    
    return future;
}

template <class R,class F>
static auto Run_2(F &&f) 
    -> QFuture<decltype(f())>
{        
    QEventLoop wait;
    
    QFutureWatcher<R> futureWatcher;
    
    auto future = QtConcurrent::run(std::forward<F>(f));
    futureWatcher.setFuture(future);
    
    QObject::connect(&futureWatcher, &QFutureWatcher<F>::finished, &wait, &QEventLoop::quit);
    wait.exec();
    QObject::disconnect(&futureWatcher, &QFutureWatcher<F>::finished, &wait, &QEventLoop::quit );
    return future;
}


class TestAcyncCall
{
    void loadDataUrl(QString apiUrl)
    {
        //    qDebug() << "call loadDataUrl";
        
        auto *requester = new HttpRequester(nullptr);
        /*  
        requester->initRequester(apiUrl);
          requester->sendRequest(
              "",
              [=](QByteArray data) {
                 qDebug()<< "  loaded " << apiUrl << data.length();          
             },
              [](QByteArray res) { qDebug() << "error callback"; }, HttpRequester::Type::GET);
          */
        
        requester->initRequester(apiUrl);
        QByteArray data = requester->sendRequestWait("");
        qDebug()<< "  loaded " << apiUrl << data.length();
        
        qDebug() << "exit Load func";  
    }
    
    
    //static auto Run(F&& f, Args&&... args)
    //    -> QFuture<decltype(f(std::forward<args>(args)...))>
    
    void testAsync()
    {
        int resVal =10;
        
        qDebug() << "before testAsync ============================";
        
        loadDataUrl("https://amazon.com/");
        //ok: auto p = Run([&resVal, this](int i){this->heavyFunction(i);}, 1234);
        //ok  auto p = Run([&resVal, this](int i){resVal = this->heavyFunction(resVal+i);}, 1234);
        
        //    auto p = Run([&resVal, this](int i){resVal = this->heavyFunction(resVal+i);}, 1234);
        // main ok :   auto p2 =  Run_2<QString>([=](){return this->heavyFunction(100, "heavy res");});
        
        auto p2 =  Run_2<QString>([=](){return this->heavyFunction(100, "heavy res");});
        
        //    qDebug() <<p2.result() << " / after RUN==========================" ;
        qDebug() << "after testAsync ==========================" ;
        //    this->set_renderTime(resVal);
        
    }
    
    QString heavyFunction(int val, QString sVal){
        qDebug() <<"heavy: " << val ;  
        int count = 10;
        for(int i=val; i<val+count; ++i) {        
            qDebug() << i;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        
        return QString("%1: %2").arg(sVal).arg(val+count);
    }
    
    
    
};

#endif // TESTASYNCCALL_H

