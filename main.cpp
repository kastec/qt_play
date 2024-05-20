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
#include "Utils/JsonHelper/FileHelper.h"

#include "Components/Test/derivedDeserializeTest.h"
#include "DataSources/appMessageBus.h"
#include "Utils/internetChecker.h"
#include "Utils/datasamples.h"
#include "qdebug.h"

#include <QObject>

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
  
  SpriteData(){qDebug()<<"ctr";}
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


bool calcSeatOrderValue(QString seatNumber){
  if(seatNumber.isEmpty()){
      return 0;
  }
  QString salon ="W";
  
  // надо сортировать по классу (salon) и по сидению
  auto letter = seatNumber.at(seatNumber.length()-1);
  auto num = seatNumber.left(seatNumber.length()-1);
  int rowNum = num.toInt();
  int letterValue = letter.unicode()-'A';
  
  int salonValue = (salon[0].unicode()-'A');
  auto seatOrderValue = (rowNum * 100 +letterValue)*100 + salonValue;
  return seatOrderValue;
}

void derivedDeserTest()
{
  auto rawData = FileHelper::readAll("C:\\Projects\\Aurora\\derivedTest.json");
  auto list = PersonsReportList::toObject(rawData);
  
  if(list!=nullptr)
      qDebug()<<"cnt:" << list->items.length();
  
  auto sdata = JsonHelper::serialize(list);
  FileHelper::save("C:\\Projects\\Aurora\\derivedTest_s.json", sdata);
}

QList<SpriteData*> filltest()
{
  QList<SpriteData*>  l;
  l.append(new SpriteData(11,"11"));
  l.append(new SpriteData(22,"22"));
  qDebug()<< "ret list" << &l;
  qDebug()<< " list[0]" << &l[0];
  return l;
}

SpriteData filltest2()
{
  auto r = filltest();
  qDebug()<< "got R list" << &r;
  qDebug()<< " R list[0]" << &r[0];
  return *r[0];
}


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


int main(int argc, char *argv[])
{
 
  
//  derivedDeserTest();  
//  return 0;
  
//  auto e = calcSeatOrderValue("2F");
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

}
