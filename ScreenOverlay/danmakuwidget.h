#ifndef DANMAKUWIDGET_H
#define DANMAKUWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "danmakutextfloatset.h"
#include "danmakutexttopset.h"
#include "danmakutextbottomset.h"
#include "danmakutestset.h"
#include "DanmakuConstants.h"

class DanmakuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DanmakuWidget(QWidget *parent = nullptr);
    ~DanmakuWidget();

    const int UPDATE_INTERVAL = 1000/DANMAKU_FPS;
    const int PAINT_INTERVAL  = 1000/DANMAKU_FPS;

    int appendFloat(const QString &text, const QColor &color = Qt::cyan, int size=20);
    int appendTop(const QString &text, const QColor &color = Qt::magenta, int size=20);
    int appendBottom(const QString &text, const QColor &color = Qt::yellow, int size=20);

protected Q_SLOTS:
    bool updateText();
    bool updateDisplay();
    void paintEvent(QPaintEvent *paint_event) override;
    void resizeEvent(QResizeEvent *resize_event) override;

    void onconnect();
    void ondisconnect();
    void reconnect();
    void onBinaryMessageRecieved(const QByteArray &message);
    void onTextMessageReceived(const QString &message);

private:
    DanmakuTextFloatSet *m_textFloatSet;
    DanmakuTextTopSet *m_textTopSet;
    DanmakuTextBottomSet *m_textBottomSet;
    QTimer *update_timer, *reconnect_timer;
    DanmakuTestSet *m_test;
    QWebSocket *m_websocket;
    QString m_url;
};

#endif // DANMAKUWIDGET_H
