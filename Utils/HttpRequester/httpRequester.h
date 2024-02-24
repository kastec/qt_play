#ifndef REQUESTER_H
#define REQUESTER_H

#include <QObject>
#include <QBuffer>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <functional>

// https://habr.com/ru/articles/314932/

class HttpRequester : public QObject
{
    Q_OBJECT
  public:
    
    typedef std::function<void(const QByteArray &)> handleFunc;
    typedef std::function<void()> finishFunc;
    
    static const QString KEY_QNETWORK_REPLY_ERROR;
    static const QString KEY_CONTENT_NOT_FOUND;
    
    
    enum class Type {
        POST,
        GET,
        PATCH,
        DELET
    };
    
    HttpRequester();
    explicit HttpRequester(QObject *parent = nullptr);
    
    void initRequester(const QString& baseUrl, QSslConfiguration *value=nullptr);
    
    void post(const QString &apiStr,
              const handleFunc &funcSuccess,
              const handleFunc &funcError,
              QObject &data);
    
    void sendRequest(const QString &apiStr,
                     const handleFunc &funcSuccess,
                     const handleFunc &funcError,
                     Type type = Type::GET,
                     const QByteArray &data = QByteArray());
    
    QByteArray sendRequestWait(const QString &apiStr,
                               HttpRequester::Type type = Type::GET,
                               const QByteArray &postDataByteArray = QByteArray());
    //  void sendMulishGetRequest(const QString &apiStr,
    //                            const handleFunc &funcSuccess,
    //                            const handleFunc &funcError,
    //                            const finishFunc &funcFinish);
    
    QString getToken() const;
    void setToken(const QString &tokenType, const QString &token);
    
    
  private:
    static const QString httpTemplate;
    static const QString httpsTemplate;
    
    QString baseUrl;
    
           //QString host;
    // int port;
    QString token;
    QString tokenType;
    
    QSslConfiguration *sslConfig;
    
    QString pathTemplate;
    
    QByteArray variantMapToJson(QVariantMap data);
    
    QNetworkRequest createRequest(const QString &apiStr);
    
    QNetworkReply *sendCustomRequest(QNetworkAccessManager *manager,
                                     QNetworkRequest &request,
                                     const QString &type,
                                     const QByteArray &postDataByteArray);
    
           //  QJsonObject parseReply(QNetworkReply *reply);
    
    bool onFinishRequest(QNetworkReply *reply);
    
    void handleQtNetworkErrors(QNetworkReply *reply);
    QNetworkAccessManager *manager;
    
  signals:
    void networkError();
    
  public slots:
    
};

#endif // REQUESTER_H
