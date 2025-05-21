#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QRegularExpression>
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

//#include "Components/Test/derivedDeserializeTest.h"
#include "Components/Test/reportGroupElements/flightCatering.h"
#include "Utils/JsonHelper/FileHelper.h"
#include "DataSources/appMessageBus.h"
//#include "Utils/internetChecker.h"
#include "Utils/datasamples.h"
#include "qdebug.h"
#include <QObject>

#include "Components/Catering/caterPlaneViewModel.h"
#include "Components/Catering/caterSeatViewModel.h"

#include <ranges>

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

void ranges20Test()
{
    
    std::vector<int> numbers = {4, 2, 7, 1, 5, 3, 6};    
                                                      // Фильтрация чётных чисел и сортировка с использованием ranges
    auto even_numbers = numbers | std::views::filter([](int x) { return x % 2 == 0; })
                        | std::views::transform([](int x) { return x * 2; });
    
           // Вывод результата
    for (int x : even_numbers) {
        qDebug() << x << " ";  // Будет: 8 4 12 (2*4, 2*2, 2*6)
    }
    
           // Сортировка с использованием ranges (на месте)
    std::ranges::sort(numbers);
    
    qDebug() << "\nAfter sort: ";
    for (int x : numbers) {
        qDebug() << x << " ";  // 1 2 3 4 5 6 7
    }
}

static int compareVersions(const QString& currentVersion, const QString& newVersion) {
    QStringList currentParts = currentVersion.split('.');
    QStringList newParts = newVersion.split('.');
    
    while (currentParts.size() < newParts.size())
        currentParts.append("0");
    while (newParts.size() < currentParts.size())
        newParts.append("0");
    
    for (int i = 0; i < currentParts.size(); ++i) {
        int curr = currentParts[i].toInt();
        int newV = newParts[i].toInt();
        qDebug()<< "c: " << curr << "n:" << newV;
        
        if (newV > curr){
            qDebug()<<" 111 ";
            return 1;
        }
        else if (newV < curr)
        {
            qDebug()<<" ---111 ";
            return -1;
        }
    }
    return 0;
}


int main(int argc, char *argv[])
{
    QString c = "1.0.15.47";
    auto r = compareVersions(c, "1.0.15.87");
    qDebug() << r;
    
    //ranges20Test();
    
           //  int t = 213;
           //  qDebug()<< QString::number(t, 16).rightJustified(2,'0');
           
    
    //   return 0;
    //  auto spo = new SpriteData();
    //  SP<SpriteData> sp(new SpriteData());
    //  spo->mystr
    //  sp->mystr
    
    QGuiApplication app(argc, argv);
    
    QQmlApplicationEngine engine;
    
    
    //    auto ws = new WebSocketClient("https://kontron-box.pifet.ru/?EIO=4&transport=websocket");
    //    auto ws = new WebSocketClient("https://kontron-box.pifet.ru/?EIO=4&transport=websocket");
    // auto ws = new WebSocketClient("wss://kontron-box.pifet.ru/socket.io/?EIO=4&transport=websocket");
    //FutureManagerTest().startTest3();
    //FutureManager2Test().startTest2();
    
    //    auto ch = InternetChecker::i();
    //    ch->startMonitoring();
    //    QObject::connect(ch, &InternetChecker::connected, []() {qDebug()<<"Internet - connected - CLIENT";});
    //    QObject::connect(ch, &InternetChecker::disconnected,  []() {qDebug()<<"Internet - disconnetct - CLIENT";});
    
    auto data = FileHelper::readAll("C:\\Projects\\Aurora\\0_VM_comm\\data\\flights-data\\20241010-204\\20241010-204-menu-catering.json");
    //    qDebug()<< "catl-1";
    auto catering = FlightCatering::toObject(data);
    //    qDebug()<<"catl-2";     
    
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
