#include "danmakutextfloat.h"

DanmakuTextFloat::DanmakuTextFloat(QObject *parent) : DanmakuText(parent)
{

}

bool DanmakuTextFloat::update()
{
    if(m_delTag) return false;
    m_pos += m_vel;
    return true;
}
