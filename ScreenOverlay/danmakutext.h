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

protected:
    QPointF     m_pos, m_vel; // position and velocity
    QString     m_text;
    QColor      m_color;
    int         m_font_size;
    QRect       m_bound;
    std::shared_ptr<QImage>      m_bufferImage;

    clock_t     m_last_clock;

    bool        m_boundReady;
    bool        m_bufferImageReady;
    bool        m_delTag;
    int         m_id;

public:
    explicit DanmakuText(QObject *parent = nullptr);

    bool setText(const QString &text);

    bool setPos(const QPointF &pos);
    bool setVel(const QPointF &vel);
    bool setColor(const QColor &color);
    bool setID(int id);
    void del();

    QPoint pos() const;
    QPointF posF() const;

    QRect bound() const;
    bool  boundReady() const;

    int id() const;

public:
    virtual bool paint(QPainter *painter);
    virtual bool update();
    virtual void calcBound(QPainter *painter);

protected:
    virtual clock_t getFrameInterval();

signals:

};

#endif // DANMAKUTEXT_H
