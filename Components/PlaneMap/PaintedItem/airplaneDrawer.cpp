#include "QDebug"

#include "Utils/HttpRequester/httpRequester.h"
#include "airplaneDrawer.h"
#include "qfuture.h"
#include "qfuturewatcher.h"
#include <QImageReader>
#include <QPainterPath>
#include <QPainter>

#include <QtConcurrent/QtConcurrentRun>

#include <future>
#include <iostream>
#include <thread>
#include <QFutureWatcher>
#include <functional>
#include "./src/planeLayoutParser.h"

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


void AirplaneDrawer::loadDataUrl(QString apiUrl)
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
    
void AirplaneDrawer::testAsync()
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
    this->set_renderTime(resVal);

}

QString AirplaneDrawer::heavyFunction(int val, QString sVal){
    qDebug() <<"heavy: " << val ;  
    int count = 10;
    for(int i=val; i<val+count; ++i) {        
        qDebug() << i;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    
    return QString("%1: %2").arg(sVal).arg(val+count);
}




AirplaneDrawer::AirplaneDrawer(QQuickItem *parent) : QQuickPaintedItem(parent), position(QPoint(0,0)), zoom(1.0)
{  
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    
 //TODO: load planeLayoutDescription, pass it to createLayout
//    planeMap = new PlaneMap();
//    planeMap->createLayout();
}


    
void AirplaneDrawer::changeVisibleSize(QSize size){
    screenSize = size;
    
    if(planeMap!=nullptr)
        planeMap->setScreenSize(screenSize);
        
    auto screenWidth = screenSize.width();

    auto chairWidth = PlaneLayoutParser::CHAIR_SIZE;
    maxZoom = screenWidth / (qreal)(3*chairWidth); // кол-во кресел * 100
    
    auto planeWidth =(12 *chairWidth); // ширина борта в пикселях (кол-во кресел * 100)
    minZoom = screenWidth / (qreal)(4*planeWidth); // 2 ширины борта  убирается на экране
}

// --- Props
bool AirplaneDrawer::onPositionChanging(QPoint newPos){
    position = newPos;
  //  qDebug() <<"pos:"<< newPos ;
    update();
    return true;
}

bool AirplaneDrawer::onZoomChanging(qreal newZoom){        
    auto zoomFactor = (newZoom-zoom)/zoom;
    bool res = zoomBy(zoomFactor,screenSize.width()/2, screenSize.height()/2);
    return res; 
}

//---------

void AirplaneDrawer::paint(QPainter *painter)
{
     auto xpos = position.x();
     auto ypos = position.y();

     framerate.start();
     
     // PLANE RENDER
     if(planeMap!=nullptr)
        planeMap->draw(painter,xpos, ypos, zoom);
     
     framerate.stop();
     
     this->set_avgRenderTime(framerate.avgFrameMsec);
     this->set_renderTime(framerate.frameMsec);

     return; 
}


void AirplaneDrawer::moveBy(qreal xOff, qreal yOff) {
    qreal ox, oy;

    //qDebug() << xOff <<","<<yOff;

    ox = position.x() - xOff;
    oy = position.y() - yOff;
//    if (ox < 0) ox = 0;
//    if (oy < 0) oy = 0;

    this->set_position(QPoint(ox,oy));   
}

bool AirplaneDrawer::zoomBy(qreal zoomFactor, qreal centerX, qreal centerY)
{
    auto newZoom = zoom + zoom * zoomFactor;
    if(newZoom>maxZoom || newZoom<minZoom) return false;
    zoom = newZoom;

    position -= QPoint( (centerX-position.x())*zoomFactor , (centerY-position.y())*zoomFactor);
    
    update();
    return true;
}


QPoint AirplaneDrawer::getMoveToCenterAt(QString id, qreal viewZoom)
{
    if(planeMap==nullptr) return QPoint(0,0);

//    auto item = planeMap->findItem(id);
    auto item = planeMap->findChair(id);
    if(item == nullptr)
        return position;
    
    if(viewZoom==0.0) viewZoom=this->zoom;
    
    auto itemCenter = item->location.center();
//    qDebug() << "zoom:" << viewZoom;
    auto itemOnScr = (itemCenter* viewZoom);
    auto tlWin = itemOnScr - boundingRect().center().toPoint();
    auto newPosition = -tlWin;

    return newPosition;
} 

QString AirplaneDrawer::getIdAt(int x, int y){
    QPoint p( (x - position.x())/ zoom, (y-position.y())/ zoom );
    auto item = planeMap->findItem(p);
    return (item!=nullptr)? item->id : "";
}
