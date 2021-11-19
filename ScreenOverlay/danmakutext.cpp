#include "danmakutext.h"

#include <QtDebug>

DanmakuText::DanmakuText(QObject *parent) : QObject(parent)
  ,m_pos(QPointF(0.0,0.0))
  ,m_vel(QPointF(0.0,0.0))
  ,m_text(QString("123"))
  ,m_color(QColor(0xff,0xff,0xff))
  ,m_boundReady(false)
  ,m_delTag(false)
{

}

bool DanmakuText::setText(const QString &text)
{
    m_text = text;
    m_boundReady = false;
    return true;
}

bool DanmakuText::setPos(const QPointF &pos)
{
    m_pos = pos;
    m_boundReady = false;
    return true;
}

bool DanmakuText::setVel(const QPointF &vel)
{
    m_vel = vel;
    return true;
}

bool DanmakuText::setColor(const QColor &color)
{
    m_color = color;
    return true;
}

bool DanmakuText::setID(int id)
{
    m_id = id;
    return true;
}

void DanmakuText::del()
{
    m_delTag = true;
}

QPoint DanmakuText::pos() const
{
    return QPoint(m_pos.x(), m_pos.y());
}

QPointF DanmakuText::posF() const
{
    return m_pos;
}

QRect DanmakuText::bound() const
{
    return m_bound;
}

bool DanmakuText::boundReady() const
{
    return m_boundReady;
}

int DanmakuText::id() const
{
    return m_id;
}

bool DanmakuText::paint(QPainter *painter)
{
    painter->setPen(m_color);
    painter->drawText(m_bound, Qt::AlignCenter, m_text);
    return true;
}

bool DanmakuText::update()
{
    if(m_delTag) return false;
    return true;
}

void DanmakuText::calcBound(QPainter *painter)
{
    m_bound = painter->fontMetrics().boundingRect(m_text);
    m_bound.moveTo(m_pos.x(),m_pos.y());
    m_boundReady = true;
}
