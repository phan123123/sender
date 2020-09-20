#include "client.h"

client::client(const QUrl &url, bool debug, int role, QObject *parent):m_url(url), _debug(debug), QObject(parent), role(role)
{
    if (_debug)
    {
        qDebug() << "websocket server:" << url;
    }
    this->connected = false;
    connect(&m_ws, &QWebSocket::connected, this, &client::onConnected);
    connect(&m_ws, &QWebSocket::disconnected, this, &client::closed);
    connect(&m_ws, &QWebSocket::textMessageReceived, this, &client::onMessage);
    m_ws.open(url);
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    const int timeout = 1000;
    timer.start(timeout);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(&m_ws, &QWebSocket::connected, &loop, &QEventLoop::quit);
    loop.exec();
    if (!timer.isActive())
        {
        qDebug() << "can't connect to server";
        return;
        }
}

void client::reconnect()
{
    m_ws.open(m_url);
}

client::client(QObject *parent):QObject(parent)
{
    m_ws.open(QUrl(""));
}

void client::init(const QUrl &url, bool debug, int role)
{

    m_url = url; _debug = debug;
    this->role = role;
    if (_debug)
    {
        qDebug() << "websocket server:" << url;
    }
    this->connected = false;
    connect(&m_ws, &QWebSocket::connected, this, &client::onConnected);
    connect(&m_ws, &QWebSocket::disconnected, this, &client::closed);
    connect(&m_ws, &QWebSocket::textMessageReceived, this, &client::onMessage);
    m_ws.open(url);
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    const int timeout = 1000;
    timer.start(timeout);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(&m_ws, &QWebSocket::connected, &loop, &QEventLoop::quit);
    loop.exec();
    if (!timer.isActive())
        {
        qDebug() << "can't connect to server";
        return;
        }
}

void client::onConnected()
{
    if (_debug)
        qDebug() << "webscoket connected" ;
    QJsonObject handshake = QJsonObject();
    handshake.insert("role", role);
    QJsonDocument doc(handshake);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    m_ws.sendTextMessage(strJson);
    this->connected = true;
    qDebug() << this->connected;
    emit opened();
}

QJsonObject client::getJson()
{
    return _json;
}

void client::send()
{

    QJsonDocument doc(_json);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    m_ws.sendTextMessage(strJson);
    print_json();

}

void client::clear()
{
     _json = QJsonObject();
}

void client::print_json()
{
    QJsonDocument doc(_json);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    qDebug() << "send : " << strJson;
}

void client::addToMessage(QString _k, QString _v)
{

    _json.insert(_k, _v);
    if (_debug)
        qDebug() << "insert key : " + _k + " value : " + _v;
}

void client::onMessage(QString mess)
{
    _rjson = QJsonObject();
    QJsonDocument doc = QJsonDocument::fromJson(mess.toUtf8());

       // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
           _rjson = doc.object();
           qDebug() << "Received" << endl;
        }
        else
        {
           qDebug() << "Document is not an object" << endl;
        }
    }
    else
    {
        qDebug() << "Invalid JSON...\n" << mess << endl;
    }
    emit received(_rjson);
}

QJsonObject client::readJsonObject()
{
    return _rjson;
}





