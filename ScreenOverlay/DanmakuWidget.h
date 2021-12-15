#ifndef DANMAKUWIDGET_H
#define DANMAKUWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QThread>
#include "DanmakuTextFloatSet.h"
#include "DanmakuTextTopSet.h"
#include "DanmakuTextBottomSet.h"
#include "DanmakuTestSet.h"
#include "DanmakuConstants.h"


#define DANMAKU_FLOAT 0
#define DANMAKU_TOP 1
#define DANMAKU_BOTTOM 2

class DanmakuAsyncRender : public QObject
{
    Q_OBJECT
public:
    explicit DanmakuAsyncRender(QObject *parent = nullptr);

public Q_SLOTS:
    void createDanamku(const QString &text, const QString &id, const QColor &color, int size, DanmakuTextSet *textset, int type);
};

class DanmakuWidget : public QWidget
{
    Q_OBJECT
signals:
    void asyncCreateText(const QString &text, const QString &id, const QColor &color, int size, DanmakuTextSet *textset, int type);
public:
    explicit DanmakuWidget(QWidget *parent = nullptr);
    ~DanmakuWidget();

    const int UPDATE_INTERVAL = 1000/DANMAKU_FPS;
    const int PAINT_INTERVAL  = 1000/DANMAKU_FPS;

protected:
    QString getWSurl();

protected Q_SLOTS:
    bool updateText();
    bool updateDisplay();
    void paintEvent(QPaintEvent *paint_event) override;
    void resizeEvent(QResizeEvent *resize_event) override;

public:
    enum textPosition{
        floatText,
        topText,
        bottomText,
    };

public Q_SLOTS:
    void onJsonMessageRecieved(const QJsonObject &obj);
    void appendText(const QString &content, const QString id, const QColor &color=QColor(0xFFFFFF), int size=20, int position=floatText);

private:
    DanmakuTextFloatSet *m_textFloatSet;
    DanmakuTextTopSet *m_textTopSet;
    DanmakuTextBottomSet *m_textBottomSet;
    QTimer *update_timer, *reconnect_timer, *shutdown_timer;
    DanmakuTestSet *m_test;
    QWebSocket *m_websocket;
    QString m_roomid, m_token;
    QThread m_renderThread;
    DanmakuAsyncRender m_asyncRender;

    bool m_wsOn,m_shutdown, m_realshutdown;
};

#endif // DANMAKUWIDGET_H
