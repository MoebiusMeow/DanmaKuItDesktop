#include "DanmakuTextBottom.h"

DanmakuTextBottom::DanmakuTextBottom(QObject *parent) : DanmakuText(parent)
{

}

bool DanmakuTextBottom::setLife(int life)
{
    if(life < 0) return false;
    m_life = life;
    return true;
}

bool DanmakuTextBottom::update()
{
    m_life --;
    if(m_life <= 0) return false;
    return DanmakuText::update();
}

void DanmakuTextBottom::calcBound()
{
    DanmakuText::calcBound();
    m_bound.moveTo(m_pos.x() - m_bound.width()/2, m_pos.y()-m_bound.height()/2);
}
