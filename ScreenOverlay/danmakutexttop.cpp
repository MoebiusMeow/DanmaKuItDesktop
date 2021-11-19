#include <QDebug>

#include "danmakutexttop.h"

DanmakuTextTop::DanmakuTextTop(QObject *parent) : DanmakuText(parent)
  , m_life(200)
{

}

bool DanmakuTextTop::setLife(int life)
{
    if(life < 0) return false;
    m_life = life;
    return true;
}

bool DanmakuTextTop::update()
{
    m_life --;
    if(m_life <= 0) return false;
    return true;
}

bool DanmakuTextTop::paint(QPainter *painter)
{
    painter->setPen(m_color);
    painter->drawText(m_bound, Qt::AlignCenter, m_text);
    return true;
}

void DanmakuTextTop::calcBound(QPainter *painter)
{
    m_bound = painter->fontMetrics().boundingRect(m_text);
    m_bound.moveTo(m_pos.x() - m_bound.width()/2, m_pos.y()-m_bound.height()/2);
    Q_UNUSED(painter);
}
