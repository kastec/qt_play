
#ifndef INTERNETCHECKER_H
#define INTERNETCHECKER_H


#include <QTcpSocket>
#include "Utils/JsonHelper/qqmlhelper.h"
//#include <QNetworkConfiguration>

class InternetChecker : public QObject{
    Q_OBJECT
    
    SINGLETON_PATTERN_DECLARE(InternetChecker)
    
    QML_READ_PROPERTY(bool, isOnline)
    QML_WRITABLE_PROPERTY_CB(bool, allowConnection, onAllowConnectionChanging)
    
  private:
    // до какого домена открываем сокет
    static QString CONN_DOMAIN;
    static int CONN_DOMAIN_PORT;
    static int CONN_TIMEOUT_sec;
    // проверяем подключение сети - напр. Wifi (wlan0) или 4G
    static QString MONITOR_NETWORK_NAME;
    
           // задержки проверок при увеличении попыток подключения
    static QList<int> attemptTimeoutsSec;
    
    QTcpSocket socket;
    
//    QNetworkConfiguration netConf;
    
  signals:
    void connected();
    void disconnected();  
    
  public:
    InternetChecker();
    void startMonitoring();
    bool checkInternetConnection();
    
    void setAllowConnection(bool isOnline);
    ~InternetChecker();
    
  private:
    void init();
    bool openSocket(QTcpSocket &sckt);
    void setSocketSettings(QTcpSocket &s);
    bool waitLoopOnline();
    void socketConnected();
    void socketDisconnected();
    
    bool onAllowConnectionChanging(bool allowConn);
    
    bool isConnectedToNetwork(QString name);
    static bool showConnectedToNetwork();        
};
#endif // INTERNETCHECKER_H
