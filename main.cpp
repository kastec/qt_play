#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Classes/bookitem.h"
#include "Classes/booklist.h"
#include "Components/PersonList/person.h"
#include "Components/PersonList/personlistmodel.h"
#include "Components/PlaneMap/PaintedItem/paintArea.h"
#include "Components/Test/jsonlistmodel.h"
#include "Components/Flights/flightslistmodel.h"
#include "Components/PlaneMap/liveimageprovider.h"
#include "Components/PlaneMap/colorImageProvider.h"

#include "Components/PlaneMap/OpGlItem/glitem.h"
#include "Components/PlaneMap/PaintedItem/airplaneViewModel.h"
#include "Utils/InternetChecker.h"
#include "Utils/sharedListItem.h"

#include "Components/Test/derivedDeserializeTest.h"
#include "DataSources/appMessageBus.h"
#include "Utils/internetChecker.h"
#include "Utils/datasamples.h"
#include "qdebug.h"
#include <QObject>

#include "Components/Catering/caterPlaneViewModel.h"
#include "Components/Catering/caterSeatViewModel.h"

struct package_manager {
  static constexpr auto package_name = "com.QtTestapp.package";
  static constexpr auto package_version_major = 0;
  static constexpr auto package_version_minor = 1;
};


void RegisterTypes();


static QString ptrToStr(void *p){
  auto str = QString::number((std::uintptr_t)(p));
  return str;
}

class SpriteData{
public:
  uint spec;
  QString mystr;
  
  SpriteData(){qDebug()<<"ctor";}
  SpriteData(int i, QString s){        
      spec =i, mystr=s;
      qDebug()<<"s:" << spec << ptrToStr((void *)this);     
  }
  ~SpriteData(){
      qDebug()<<"DESTR" << spec;
  }
};

QSharedPointer <SpriteData> test1()
{  
  auto sd = new SpriteData();
  qDebug()<< "data" << sd;
  sd->spec=1111;
  
  QSharedPointer <SpriteData> scp(sd) ;
  
//  QString("")
  qDebug()<< "t2:       " << scp->spec << &scp << "(data:"<<scp.data()<<")";
  return scp;
};

void test2()
{
  auto sp2 = test1();
  qDebug()<<"t2-shared:" << sp2->spec << &sp2<< "(data:"<<sp2.data()<<")";
  
  auto scop = QScopedPointer<SpriteData>(new SpriteData{2,""});
  qDebug()<< "t2-scoped:" << scop->spec;
};

class Storager{
public:
  SharedQList<SpriteData> data;
//  Storager(){
      
//  };
//  Storager(SharedQList<SpriteData> d):data(d){}
};


void testSplit()
{
  QList<SpriteData*> l;
  l.append(new SpriteData(1,"11"));
  l.append(new SpriteData(2,"22"));
  l.append(new SpriteData(3,"33"));
  
  auto [a,b] = split<SpriteData*>(l, [](const auto &s){
      qDebug() << s->mystr <<  ptrToStr((void *)&s);
      return s->spec%2==1; });
  
  qDeleteAll(b); 
}

SharedQList<SpriteData> TestSharedListItems0()
{

  SharedQList<SpriteData> l;
  l->append(new SpriteData(1,"11"));
  l->append(new SpriteData(1,"11"));
  l->append(new SpriteData(2,"22"));
  l->append(new SpriteData(3,"33"));
  
  //  auto sharedList = new SharedQList<SpriteData>(l);
//  SharedQList<SpriteData> sharedList(l);
  qDebug()<<"exit test-0";
  return l;
}
void TestSharedListIncome(SharedQList<SpriteData> sp)
{ 
  QList<SpriteData*> *lst = sp.data();
  qDebug()<<"income list: "<<  ptrToStr(lst);
}

void TestSharedListItems()
{
  qDebug()<<"enter test";

  auto stor = new Storager();
  stor->data = TestSharedListItems0();
  delete stor;
  qDebug()<<"exit test";
}

int main(int argc, char *argv[])
{
  DeserializeTest dest;
  dest.Test();
//  TestSharedListItems();
//  derivedDeserTest();  
  return 0;
  

//   return 0;
//  auto spo = new SpriteData();
//  SP<SpriteData> sp(new SpriteData());
//  spo->mystr
//  sp->mystr
  
  
 
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    
//    auto ch = InternetChecker::i();
//    ch->startMonitoring();
//    QObject::connect(ch, &InternetChecker::connected, []() {qDebug()<<"Internet - connected - CLIENT";});
//    QObject::connect(ch, &InternetChecker::disconnected,  []() {qDebug()<<"Internet - disconnetct - CLIENT";});
     
 
    RegisterTypes();
    
    /// test image proviger for PlaneMap
    QScopedPointer<LiveImageProvider> liveImageProvider(new LiveImageProvider());
    engine.rootContext()->setContextProperty("liveImageProvider", liveImageProvider.data());
    engine.addImageProvider(QLatin1String("colors"), new ColorImageProvider);
    
    engine.addImageProvider("live", liveImageProvider.data());
    
       
  // auto jsonPers = DataSamples::LoadJsonData();
  //  qDebug() << &jsonPers;
    
    auto q = app.devicePixelRatio();
    
    const QUrl url(u"qrc:/QtTest1/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    
   
    // test message bus
    QObject::connect(AppMessageBus::i(), &AppMessageBus::logEvent,
            [=](const QString& line) {
                qDebug() << "got SIGNAL Log: "<< line;                
            });
    
    engine.load(url);
    
//    JsonHelper::Test();
  
    return app.exec();
}



void RegisterTypes()
{    
    qmlRegisterType<GlItem>("AppQtTest12", 1, 0, "GlItem");
    qmlRegisterType<BookItem>("AppQtTest12", 1,0, "BookItem");
    qmlRegisterType<BookList>("AppQtTest12",1,0, "BookList");
    qmlRegisterType<Person>("AppQtTest12",1,0, "Person");
    qmlRegisterType<PersonListModel>("AppQtTest12",1,0, "PersonListModel");
    qmlRegisterType<JsonListModel>("AppQtTest12",1,0, "JsonListModel");
    
    qmlRegisterType<FlightsListModel>("AppQtTest12",1,0, "FlightsListModel");
    
    qmlRegisterType<AirplaneViewModel>("AppQtTest12",1,0, "AirplaneViewModel");
    qmlRegisterType<PaintArea>("AppQtTest12", 1, 0, "PaintArea");
    
    qmlRegisterSingletonType<AppMessageBus>("AppQtTest12", 1,0,"AppMessageBus", &AppMessageBus::qmlInstance);
    qmlRegisterSingletonType<InternetChecker>("AppQtTest12", 1,0,"InternetChecker", &InternetChecker::qmlInstance);
    
    qmlRegisterType<CaterPlaneViewModel>("AppQtTest12", 1, 0, "CaterPlaneViewModel");
    qmlRegisterType<CaterSeatViewModel>("AppQtTest12", 1, 0, "CaterSeatViewModel");

}
