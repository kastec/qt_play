
#ifndef WSCLIENT_H
#define WSCLIENT_H

#include <QCoreApplication>
#include <QWebSocket>
#include <QDebug>

class WebSocketClient : public QObject {
    Q_OBJECT
    
  public:
    explicit WebSocketClient(const QString &url, QObject *parent = nullptr) 
        : QObject(parent), m_webSocket() {
        
        qDebug() << "WebSocket: " << url;
        connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
        connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
        connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onMessageReceived);
        
//        connect(&m_webSocket, &QWebSocket::errorOccurred, this, &WebSocketClient::onError);
        
        connect( &m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
                [=](QAbstractSocket::SocketError error){
                    qDebug() << "[IoBoard] Error: " << error;
                }
                );
        
        m_webSocket.open(QUrl(url));
    }
    
  private slots:
    void onConnected() {
        qDebug() << "WebSocket connected";
        // Отправка сообщения после подключения
        //m_webSocket.sendTextMessage("Hello, Server!");
    }
    
    void onDisconnected() {
        qDebug() << "WebSocket disconnected";
    }
    
    void onMessageReceived(const QString &message) {
        qDebug() << "Received message:" << message;
    }
    void onError(QAbstractSocket::SocketError error) {
        qDebug() << "WebSocket Error:" 
                 << m_webSocket.errorString() 
                 << "Error code:" << error;
    }
    
  private:
    QWebSocket m_webSocket;
};


#endif // WSCLIENT_H
