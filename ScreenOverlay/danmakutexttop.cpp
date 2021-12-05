#include <QDebug>

#include "danmakutexttop.h"

DanmakuTextTop::DanmakuTextTop(QObject *parent) : DanmakuText(parent)
  , m_life(400)
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

void DanmakuTextTop::calcBound(QPainter *painter)
{
    DanmakuText::calcBound(painter);
    m_bound.moveTo(m_pos.x() - m_bound.width()/2, m_pos.y()-m_bound.height()/2);
}
