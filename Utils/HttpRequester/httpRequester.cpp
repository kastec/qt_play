#include "httpRequester.h"
#include "qdebug.h"
#include "qeventloop.h"
#include "Utils/JsonHelper/JsonHelper.h"

const QString HttpRequester::KEY_QNETWORK_REPLY_ERROR = "QNetworkReplyError";
const QString HttpRequester::KEY_CONTENT_NOT_FOUND = "ContentNotFoundError";

HttpRequester::HttpRequester(): HttpRequester(nullptr)
{
    
}

HttpRequester::HttpRequester(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}


void HttpRequester::initRequester(const QString &baseUrl, QSslConfiguration *value)
{
    this->baseUrl = baseUrl;
    sslConfig = value;
}


void HttpRequester::post(const QString &apiStr,
                         const handleFunc &funcSuccess,
                         const handleFunc &funcError,
                         QObject &data)
{
    auto bytes = JsonHelper::serialize(&data);
    this->sendRequest(apiStr, funcSuccess, funcError, Type::POST, bytes);
}

void HttpRequester::sendRequest(const QString &apiStr,
                                const handleFunc &funcSuccess,
                                const handleFunc &funcError,
                                HttpRequester::Type type,
                                const QByteArray &postDataByteArray)
{
    QNetworkRequest request = createRequest(apiStr);
    
    QNetworkReply *reply;
    switch (type) {
    case Type::POST: {
        //QByteArray postDataByteArray = variantMapToJson(data);
        reply = manager->post(request, postDataByteArray);
        break;
    } case Type::GET: {
        reply = manager->get(request);
        break;
    } case Type::DELET: {
        if (postDataByteArray.isEmpty())
            reply = manager->deleteResource(request);
        else
            reply = sendCustomRequest(manager, request, "DELETE", postDataByteArray);
        break;
    } case Type::PATCH: {
        reply = sendCustomRequest(manager, request, "PATCH", postDataByteArray);
        break;
    } default:
        reply = nullptr;
        Q_ASSERT(false);
    }
    
    qDebug() << "http req start" << request.url();
    
    connect(reply, &QObject::destroyed, [] { qDebug() << "QNetworkReply got deleted!"; });
    
    connect(reply, &QNetworkReply::finished, this,
            [this, funcSuccess, funcError, reply]() {
                qDebug() << "http req finished";
                
                qDebug() << "parsing";
                //auto obj = parseReply(reply);
                auto replyText = reply->readAll();
                
                if (onFinishRequest(reply)) {
                    if (funcSuccess != nullptr)
                        funcSuccess(replyText);
                } else {
                    if (funcError != nullptr) {
                        handleQtNetworkErrors(reply);
                        funcError(replyText);
                    }
                }
                reply->close();
                reply->deleteLater();
                this->deleteLater();
            } );
    //  qDebug() << "exit call http";
}



// уже наша дописка
QByteArray HttpRequester::sendRequestWait(const QString &apiStr,
                                          HttpRequester::Type type,
                                          const QByteArray &postDataByteArray)
{
    QNetworkRequest request = createRequest(apiStr);
    
    QNetworkReply *reply;
    switch (type) {
    case Type::POST: {
        //QByteArray postDataByteArray = variantMapToJson(data);
        reply = manager->post(request, postDataByteArray);
        break;
    } case Type::GET: {
        reply = manager->get(request);
        break;
    } case Type::DELET: {
        if (postDataByteArray.isEmpty())
            reply = manager->deleteResource(request);
        else
            reply = sendCustomRequest(manager, request, "DELETE", postDataByteArray);
        break;
    } case Type::PATCH: {
        reply = sendCustomRequest(manager, request, "PATCH", postDataByteArray);
        break;
    } default:
        reply = nullptr;
        Q_ASSERT(false);
    }
    
    connect(reply, &QObject::destroyed, [] { qDebug() << "QNetworkReply got deleted!"; });
    
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    
    QByteArray response = reply->readAll();
    
    reply->close();
    reply->deleteLater();
    return response;
    //  qDebug() << "exit call http";
}



/*
void HttpRequester::sendMulishGetRequest(const QString &apiStr, //а ничего что здесь нигде не проверяется func != nullptr?
                                     const handleFunc &funcSuccess,
                                     const handleFunc &funcError,
                                     const finishFunc &funcFinish)
{
    QNetworkRequest request = createRequest(apiStr);
    //    QNetworkReply *reply;
    qInfo() << "GET REQUEST " << request.url().toString() << "\n";
    auto reply = manager->get(request);

 connect(reply, &QNetworkReply::finished, this,
         [this, funcSuccess, funcError, funcFinish, reply]() {
             QJsonObject obj = parseReply(reply);
             if (onFinishRequest(reply)) {
                 if (funcSuccess != nullptr)
                     funcSuccess(obj);
                 QString nextPage = obj.value("next").toString();
                 if (!nextPage.isEmpty()) {
                     QStringList apiMethodWithPage = nextPage.split("api/");
                     sendMulishGetRequest(apiMethodWithPage.value(1),
                                          funcSuccess,
                                          funcError,
                                          funcFinish
                                          );
} else {
if (funcFinish != nullptr)
funcFinish();
}
} else {
handleQtNetworkErrors(reply, obj);
if (funcError != nullptr)
funcError(obj);
}
reply->close();
reply->deleteLater();
});
}
*/

QString HttpRequester::getToken() const
{
    return token;
}

void HttpRequester::setToken(const QString &tokenType, const QString &token)
{
    this->token = token;
    this->tokenType = tokenType;
}

QByteArray HttpRequester::variantMapToJson(QVariantMap data)
{
    QJsonDocument postDataDoc = QJsonDocument::fromVariant(data);
    QByteArray postDataByteArray = postDataDoc.toJson();
    
    return postDataByteArray;
}

QNetworkRequest HttpRequester::createRequest(const QString &apiStr)
{
    QNetworkRequest request;
    QString url = baseUrl.isEmpty()
                      ? apiStr
                      : baseUrl + apiStr; // pathTemplate.arg(host).arg(port);
    
    request.setUrl(QUrl(url));
    request.setRawHeader("Content-Type","application/json");
    if(!token.isEmpty())
        request.setRawHeader("Authorization",QString("%1 %2").arg(tokenType).arg(token).toUtf8());
    if (sslConfig != nullptr)
        request.setSslConfiguration(*sslConfig);
    
    return request;
}

QNetworkReply* HttpRequester::sendCustomRequest(QNetworkAccessManager* manager,
                                                QNetworkRequest &request,
                                                const QString &type,
                                                const QByteArray &postDataByteArray)
{
    request.setRawHeader("HTTP", type.toUtf8());   
    QBuffer *buff = new QBuffer;
    buff->setData(postDataByteArray);
    buff->open(QIODevice::ReadOnly);
    QNetworkReply* reply =  manager->sendCustomRequest(request, type.toUtf8(), buff);
    buff->setParent(reply);
    return reply;
}
/*
QJsonObject HttpRequester::parseReply(QNetworkReply *reply)
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    auto replyText = reply->readAll();
    jsonDoc = QJsonDocument::fromJson(replyText, &parseError);
    if(parseError.error != QJsonParseError::NoError){
        qDebug() << replyText;
        qWarning() << "Json parse error: " << parseError.errorString();
    }else{
        if(jsonDoc.isObject())
            jsonObj  = jsonDoc.object();
        else if (jsonDoc.isArray())
            jsonObj["non_field_errors"] = jsonDoc.array();
    }
    return jsonObj;
}*/



bool HttpRequester::onFinishRequest(QNetworkReply *reply)
{
    auto replyError = reply->error();
    if (replyError == QNetworkReply::NoError) {
        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if ((code >= 200) && (code < 300)) {
            return true;
        }
    }
    return false;
}

void HttpRequester::handleQtNetworkErrors(QNetworkReply *reply)
{
    auto replyError = reply->error();
    if (!(replyError == QNetworkReply::NoError ||
          replyError == QNetworkReply::ContentNotFoundError ||
          replyError == QNetworkReply::ContentAccessDenied ||
          replyError == QNetworkReply::ProtocolInvalidOperationError
          ) ) {
        qDebug() << reply->error();
        // obj[KEY_QNETWORK_REPLY_ERROR] = reply->errorString();
    } else
        if (replyError == QNetworkReply::ContentNotFoundError)
            qDebug() << reply->errorString();
    //obj[KEY_CONTENT_NOT_FOUND] = reply->errorString();
    
}
