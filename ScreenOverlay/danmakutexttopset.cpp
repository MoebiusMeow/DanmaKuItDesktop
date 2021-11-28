#include "danmakutexttopset.h"
#include <QDebug>

DanmakuTextTopSet::DanmakuTextTopSet(QObject *parent) : DanmakuTextSet(parent)
{

}

int DanmakuTextTopSet::getRailCnt()
{
    return DANMAKU_TOP_RAIL_CNT;
}

int DanmakuTextTopSet::calcRailYpos()
{
    int deltY = m_bound.height()/2/(getRailCnt()+1);
    for(int i=0;i<getRailCnt();i++){
        m_railYpos[i] = deltY * (i+1) + m_bound.top();
    }
    return getRailCnt();
}

bool DanmakuTextTopSet::ifBlockRail(const DanmakuText &text, int railID)
{
    QRect r = text.bound();
    return r.top()<= m_railYpos[railID] && r.bottom() >= m_railYpos[railID];
}

void DanmakuTextTopSet::pushToRail(DanmakuText &text, int railID)
{
    text.setPos(QPointF((m_bound.right()+m_bound.left())>>1, m_railYpos[railID]));
    ((DanmakuTextTop *)(&text))->setLife(240);
    text.setID(railID);
}
