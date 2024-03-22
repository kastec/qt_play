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
#include "Utils/JsonHelper/FileHelper.h"

#include "Components/Test/derivedDeserializeTest.h"

struct package_manager {
  static constexpr auto package_name = "com.QtTestapp.package";
  static constexpr auto package_version_major = 0;
  static constexpr auto package_version_minor = 1;
};


void RegisterTypes();

class SpriteData{
public:
  uint spec;
  QString mystr;
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
  
  auto scop = QScopedPointer<SpriteData>(new SpriteData{2222});
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
  
  auto sdata =JsonHelper::serialize(list);
  FileHelper::save("C:\\Projects\\Aurora\\derivedTest_s.json", sdata);
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
  

// test2();
 
 
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // SharedDataContext context();
    //engine.rootContext()->setContextProperty("_context", &context);

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

}
