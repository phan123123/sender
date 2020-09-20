#ifndef CLIENT_H
#define CLIENT_H
#include <QtWebSockets/QtWebSockets>
#include <QtCore/QObject>
#include <json.hpp>
#define ROLE_SEND 0
#define ROLE_RECEIVER 1
#define ROLE_BOTH 2
class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = nullptr);
    void reconnect();
    explicit client(const QUrl &url, bool debug = false, int role = 1 ,QObject *parent = nullptr);
    void init(const QUrl &url, bool debug = false, int role = 1);
    QJsonObject getJson();
public:
    void addToMessage(QString , QString );
    void send();
    void clear();
    QJsonObject readJsonObject();
    bool connected;
Q_SIGNALS:
    void closed();
    void opened();
    void received(QJsonObject);
private:
    QWebSocket m_ws;
    QUrl m_url;
    QJsonObject _json, _rjson;
    bool _debug;
    void onConnected();
    int role;
    void print_json();

private Q_SLOTS:
    void onMessage(QString);

};

#endif // CLIENT_H
