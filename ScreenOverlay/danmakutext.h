#ifndef DANMAKUTEXT_H
#define DANMAKUTEXT_H

#include <QObject>
#include <QPointF>
#include <QPainter>
#include <ctime>

#include "DanmakuConstants.h"


class DanmakuText : public QObject
{
    Q_OBJECT
private:
    static bool s_font_loaded;

protected:
    static QFont s_default_font;
    QFont       m_font;

    QPointF     m_pos, m_vel; // position and velocity
    QString     m_text;
    QColor      m_color;
    int         m_font_size;
    QRect       m_bound;
    QImage      *m_bufferImage;

    clock_t     m_last_clock;

    bool        m_boundReady;
    bool        m_bufferImageReady;
    bool        m_delTag;
    QString     m_id;
    int         m_railid;

public:
    explicit DanmakuText(QObject *parent = nullptr);
    ~DanmakuText();

    void renderText();

    bool setText(const QString &text);

    bool setPos(const QPointF &pos);
    bool setVel(const QPointF &vel);
    bool setColor(const QColor &color);
    bool setFontSize(int size);
    bool setID(const QString &id);
    bool setRailID(int railid);
    void del();

    QPoint pos() const;
    QPointF posF() const;

    QRect bound() const;
    bool  boundReady() const;

    QString id() const;
    int railID() const;

public:
    virtual bool paint(QPainter *painter);
    virtual bool update();
    virtual void calcBound();

protected:
    virtual clock_t getFrameInterval();

signals:

};

#endif // DANMAKUTEXT_H
