#ifndef DANMAKUWIDGET_H
#define DANMAKUWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QCloseEvent>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <NetworkConfig.h>
#include <QRandomGenerator>
#include "danmakutextfloatset.h"
#include "danmakutexttopset.h"
#include "danmakutextbottomset.h"
#include "danmakutestset.h"
#include "DanmakuConstants.h"

class DanmakuWidget : public QWidget
{
    Q_OBJECT
signals:
    void wsConnected();
    void wsClosed();
    void wsForceClosed();
    void wsBroken();

public:
    explicit DanmakuWidget(QWidget *parent = nullptr);
    ~DanmakuWidget();

    const int UPDATE_INTERVAL = 1000/DANMAKU_FPS;
    const int PAINT_INTERVAL  = 1000/DANMAKU_FPS;

    int appendFloat(const QString &text, const QString &id, const QColor &color = Qt::cyan, int size=20);
    int appendTop(const QString &text, const QString &id, const QColor &color = Qt::magenta, int size=20);
    int appendBottom(const QString &text, const QString &id, const QColor &color = Qt::yellow, int size=20);

protected:
    QString getWSurl();

protected Q_SLOTS:
    bool updateText();
    bool updateDisplay();
    void paintEvent(QPaintEvent *paint_event) override;
    void resizeEvent(QResizeEvent *resize_event) override;
    void closeEvent(QCloseEvent *close_event) override;

    void onconnect();
    void ondisconnect();
    void reconnect();
    void onforceShutdown();
    void onBinaryMessageRecieved(const QByteArray &message);
    void onTextMessageReceived(const QString &message);

public Q_SLOTS:
    void wsConnect(const QString &roomid, const QString &token);
    void wsClose();

private:
    DanmakuTextFloatSet *m_textFloatSet;
    DanmakuTextTopSet *m_textTopSet;
    DanmakuTextBottomSet *m_textBottomSet;
    QTimer *update_timer, *reconnect_timer, *shutdown_timer;
    DanmakuTestSet *m_test;
    QWebSocket *m_websocket;
    QString m_roomid, m_token;

    bool m_wsOn,m_shutdown, m_realshutdown;

};

#endif // DANMAKUWIDGET_H
