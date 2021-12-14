#ifndef NETWORKAPI_H
#define NETWORKAPI_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QtWebSockets>
#include <QJsonDocument>
#include <QJsonObject>
#include "NetworkConfig.h"

class NetworkAPI : public QObject
{
    Q_OBJECT
public:
    explicit NetworkAPI(QObject *parent = nullptr);
    ~ NetworkAPI();

    void setDanmakuDomain(QString domain);


private:
    QString danmaku_domain;
    QString m_roomID;
    QString m_roomToken;
    QNetworkReply *m_reply;
    QNetworkAccessManager *m_netManager;
    QWebSocket *m_websocket;
    int m_status;
    enum status{
        logged_out,
        logging_in,
        logged_in,
        logging_out,
    };
    bool m_allowReconnect;
    QTimer *m_wsConnectionCheckTimer;

private :
    QString getWebsocketURL();

public Q_SLOTS:
    // https
    void login(const QString &id, const QString &pass );
    void logout();

    //websocket
    void wsConnect(const QString &roomid, const QString &token);
    void wsClose();

protected Q_SLOTS:
    // https
    void on_loginReplyRecieve();

    //websocket
    void on_wsRecieveBinaryMessage(const QByteArray &message);
    void on_wsRecieveTextMessage(const QString &message);
    void on_wsConnected();
    void on_wsDisConnected();
    void on_wsError(QAbstractSocket::SocketError error);
    void on_wsConnectionCheck();

    void on_setDanmakuDomain(QString domain);

signals:
    void loginSuccess();
    void logoutSuccess();
    void loginFailed(int errorType, QString errorMessage);
    void logoutFailed(QString errorMessage);
    void ConnectionAborted(QString errorMessage);
    void wsInfoReady(QString roomid, QString token);


    //websocket
    void jsonMessage(QJsonObject obj);
};

#endif // NETWORKAPI_H
