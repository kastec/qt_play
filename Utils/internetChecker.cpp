

#include <QTimer>
#include <QThread>
#include "qdebug.h"
#include <QtNetwork/QNetworkInterface>
#include <QFutureWatcher>

#include "internetChecker.h"


#ifdef Q_OS_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QtConcurrent/QtConcurrent>
#endif

#ifdef Q_OS_UNIX

#include "netinet/in.h"
#include "netinet/tcp.h"
#endif

//http://www.forum.crossplatform.ru/lofiversion/index.php/t8798.html

#ifdef Q_OS_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#ifdef Q_OS_UNIX
#include "netinet/in.h"
#include "netinet/tcp.h"
#include <QtConcurrent>
//netinet/in.h, netinet/tcp.h
//sys/socket.h
#endif

SINGLETON_PATTERN_IMPLIMENT(InternetChecker)

//QString InternetChecker::CONN_DOMAIN = "google.com";
//int InternetChecker::CONN_DOMAIN_PORT = 80; // 443 for HTTPS or use Port 80 for HTTP

QString InternetChecker::CONN_DOMAIN = "su-stage.ag-ife.com";
int InternetChecker::CONN_DOMAIN_PORT = 443; // 443 for HTTPS or use Port 80 for HTTP
int InternetChecker::CONN_TIMEOUT_sec = 3;

//QString InternetChecker::MONITOR_NETWORK_NAME= "wlan0"; // 4G ccmni1
QString InternetChecker::MONITOR_NETWORK_NAME= ""; // 4G ccmni1

QList<int> InternetChecker::attemptTimeoutsSec = {2, 5, 5, 5, 10, 10, 20, 20, 30, 30, 45, 60};

InternetChecker::InternetChecker(): isOnline(false), allowConnection(true){
    init();
}

void InternetChecker::init()
{
//    connect(&socket, &QTcpSocket::connected, InternetChecker::i(), &InternetChecker::socketConnected );
//    connect(&socket, &QTcpSocket::disconnected, InternetChecker::i(), &InternetChecker::socketDisconnected);
    
    connect(&socket, &QTcpSocket::connected, this, &InternetChecker::socketConnected );
    connect(&socket, &QTcpSocket::disconnected, this, &InternetChecker::socketDisconnected);
    
    connect(&socket, &QTcpSocket::stateChanged, this, [](QTcpSocket::SocketState st){
        qDebug()<< "  socket state: " <<st;        
    });
}

bool InternetChecker::waitLoopOnline(){
        
    int attempts=0;
    
    do
    {
        // проверим что указанный тип подключения поднят (т.е. 4G модем включен)
        bool isUpNetwork = isConnectedToNetwork(InternetChecker::MONITOR_NETWORK_NAME);
        qDebug() << "InternetChecker - check is Network " << InternetChecker::MONITOR_NETWORK_NAME << " Up: "<< isUpNetwork;
        
        if(isUpNetwork){
            bool isOnline = checkInternetConnection();
            if(isOnline) {
                qDebug() << "InternetChecker - connection detected";
                return true;
            }
        }
        
        int delaySec = (attempts < InternetChecker::attemptTimeoutsSec.length())
                           ? InternetChecker::attemptTimeoutsSec[attempts]
                           : InternetChecker::attemptTimeoutsSec.last();
        
        attempts++;
        qDebug() << "InternetChecker - Attempts:" << attempts << ", sleep " << delaySec <<" sec" << "class: "<< this;
        QThread::sleep(delaySec);
    } while(true);
    
    return false;
}


void InternetChecker::startMonitoring()
{
 
    //    showConnectedToNetwork();
    
    qDebug() << "InternetChecker - start monitoring (thr:" + QString::number((long long)QThread::currentThreadId(), 16) + ")";
    
           // поиск подключения (waitLoopOnline) работает в отельном потоке,
           // и когда определится, что подключение установлено, открываем сокет в текущем потоке
           // и подписываемся на его сигналы. Таким образом, сигналы вызаваются в одинм потоке, где и сам сокет создан
    QFutureWatcher<bool> *watcher = new QFutureWatcher<bool>(this);
    
    connect(watcher, &QFutureWatcher<bool>::finished, this, [watcher, this](){
        watcher->deleteLater();
        auto isOnline = watcher->result();
        if(isOnline){
            qDebug()<< "internet - isOnline";
            // открываем сокет в текущем потоке
            bool res = openSocket(this->socket);
            if(!res)
                startMonitoring();
        }
    });
    
    watcher->setFuture(QtConcurrent::run([this]() {return this->waitLoopOnline();}));
}

bool InternetChecker::checkInternetConnection()
{
    QTcpSocket sckt;
    
    bool isConnected = openSocket(sckt);
    sckt.close();
    return isConnected;
}

bool InternetChecker::openSocket(QTcpSocket &sckt)
{
   
    // проверим, не открыт ли уже сокет
    bool isAlreadyConnected = sckt.state() == QTcpSocket::ConnectedState;
    if(isAlreadyConnected) return true;
    
    qDebug() << "InternetChecker - connecting " << InternetChecker::CONN_DOMAIN << " (thr:" + QString::number((long long)QThread::currentThreadId(), 16) + ")";
    
    QElapsedTimer timer;
    timer.start();
    
    sckt.connectToHost(InternetChecker::CONN_DOMAIN, InternetChecker::CONN_DOMAIN_PORT);
    sckt.waitForConnected(5000);
    
    bool isConnected = sckt.state() == QTcpSocket::ConnectedState;
    qDebug() << "InternetChecker - connecting state" << sckt.state() << "in "<<timer.elapsed();
    
    return isConnected;
}

void InternetChecker::socketConnected()
{
//    connect(&this->socket, &QTcpSocket::disconnected, this, &InternetChecker::socketDisconnected);
    qDebug() << "class: cc " << this;
    
    isOnline=true;
    isOnlineChanged(true);
    
    qDebug() << "InternetChecker - socket connected, isOnline:" << isOnline;
    setSocketSettings(this->socket);
    
    emit connected();
}

void InternetChecker::setSocketSettings(QTcpSocket &skct)
{
    int fd = skct.socketDescriptor();
    qDebug() << "InternetChecker - set socket settings, socket descriptor: " << fd;
    
    int enableKeepAlive = 1;
    int maxIdle = 1; // seconds
    int count = 2;  // send up to [count] keepalive packets out, then disconnect if no response
    int interval = 1; // seconds, send a keepalive packet out every [interval] seconds (after the 5 second idle period)
    
           //    int result;
    
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (const char *) &enableKeepAlive, sizeof(enableKeepAlive));
    //    qDebug()<<"inter SO_KEEPALIVE "<< result;
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, (const char *) &maxIdle, sizeof(maxIdle));
    //    qDebug()<<"inter TCP_KEEPIDLE "<< result;
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, (const char *) &count, sizeof(count));
    //    qDebug()<<"inter TCP_KEEPCNT "<< result;
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL,(const char *) &interval, sizeof(interval));
    //    qDebug()<<"inter TCP_KEEPINTVL "<< result;
    
           //    socket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
           //    socket.setSocketOption(QAbstractSocket::LowDelayOption, 1);
}

void InternetChecker::socketDisconnected()
{
    qDebug() << "class:" << this;
    
    isOnline=false;
    isOnlineChanged(false);
    qDebug() << "InternetChecker - socket disconnected";
    
    emit disconnected();
    
    startMonitoring();
}

bool InternetChecker::onAllowConnectionChanging(bool allowConn){
    this->allowConnection = allowConn;
    
    qDebug() << "class:" << this;
    
    qDebug()<< "change AllowConn to " << allowConn << " isOnline: "<<isOnline ;
//    isOnline
//    bool isConnected = sckt.state() == QTcpSocket::ConnectedState;
    if(!allowConn && isOnline) // закрываем текущее соединение
    {
        qDebug()<< " closing socket connection";
//        connect(&socket, &QTcpSocket::disconnected, this, &InternetChecker::socketDisconnected);
        socket.abort();
//        socket.close();
    }
    
    if(allowConn && !isOnline) // проверяем доступность интернета
    {
        qDebug()<< " strating monitoring";
        startMonitoring();
    }
            
    return true;
}


// проверка сетевых подключений
bool InternetChecker::isConnectedToNetwork(QString name)
{
    if(this->allowConnection==false) return false;
    
    // если не указано ограничение по типу подключения - значит не проверяем
    if(name.isEmpty()) return true;
    
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    
    auto f = std::find_if(ifaces.begin(), ifaces.end(), [&name](QNetworkInterface &f)
                          { return f.name().startsWith(name) && f.flags().testFlag(QNetworkInterface::IsUp);});
    
    if(f != ifaces.end())
        return true;
    
    /*for (int i = 0; i < ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);
        if(iface.name().startsWith(name))
        {
            bool isUp  = iface.flags().testFlag(QNetworkInterface::IsUp);
            return isUp;

         }
         */
    return false;
}

bool InternetChecker::showConnectedToNetwork()
{
    qDebug()<< " ----- NETWORKS ------------------";
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;
    
    for (int i = 0; i < ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);
        bool isUp  = iface.flags().testFlag(QNetworkInterface::IsUp);
        bool IsLoopBack  = iface.flags().testFlag(QNetworkInterface::IsLoopBack);
        
        if (isUp && !IsLoopBack)
        {
            // details of connection
            qDebug() << "---" << iface.name() << " is UP:" << isUp;
            qDebug() << "name:" << iface.name();
            qDebug() << "mac:" << iface.hardwareAddress() ;
            /*
              qDebug() << "ip addresses:" ;
  
                      // this loop is important
               for (int j=0; j<iface.addressEntries().count(); j++)
               {
   
                    qDebug() << iface.addressEntries().at(j).ip().toString()<< " / " << iface.addressEntries().at(j).netmask().toString();
    
                            // we have an interface that is up, and has an ip address
                            // therefore the link is present
     
                             // we will only enable this check on first positive,
                             // all later results are incorrect
                      if (result == false)
                          result = true;
                  }
                  */
        }
        
    }
    qDebug()<< " ----------------------";
    return result;
}

InternetChecker::~InternetChecker()
{
    disconnect(&socket, &QTcpSocket::connected, this, &InternetChecker::socketConnected );
    disconnect(&socket, &QTcpSocket::disconnected, this, &InternetChecker::socketDisconnected);
    
    if(socket.isOpen())
        socket.abort();
}
