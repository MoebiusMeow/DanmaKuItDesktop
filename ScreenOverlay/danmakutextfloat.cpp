#include "danmakutextfloat.h"
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
