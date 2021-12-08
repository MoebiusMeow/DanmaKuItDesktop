#include "DanmakuTextFloat.h"
#include <QDebug>

DanmakuTextFloat::DanmakuTextFloat(QObject *parent) : DanmakuText(parent)
{

}

bool DanmakuTextFloat::update()
{
    if(m_delTag) return false;
    m_pos += m_vel * getFrameInterval();
    return DanmakuText::update();
}

void DanmakuTextFloat::calcBound()
{
    DanmakuText::calcBound();
    m_bound.moveTo(m_pos.x(), m_pos.y()-m_bound.height()/2);
}
