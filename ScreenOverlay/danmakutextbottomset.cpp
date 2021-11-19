#include "danmakutextbottomset.h"

DanmakuTextBottomSet::DanmakuTextBottomSet(QObject *parent) : DanmakuTextSet(parent)
{

}

int DanmakuTextBottomSet::getRailCnt()
{
    return DANMAKU_BOTTOM_RAIL_CNT;
}

int DanmakuTextBottomSet::calcRailYpos()
{
    int n = getRailCnt();
    int deltY = m_bound.height()/2/(n+1);
    int bias = m_bound.height()/2;
    for(int i=0;i<n;i++){
        m_railYpos[n-i-1] = deltY * (i+1) + m_bound.top() + bias;
    }
    return getRailCnt();
}

bool DanmakuTextBottomSet::ifBlockRail(const DanmakuText &text, int railID)
{
    QRect r = text.bound();
    return r.top()<= m_railYpos[railID] && r.bottom() >= m_railYpos[railID];
}

void DanmakuTextBottomSet::pushToRail(DanmakuText &text, int railID)
{
    text.setPos(QPointF((m_bound.right()+m_bound.left())>>1, m_railYpos[railID]));
    ((DanmakuTextBottom *)(&text))->setLife(120);
    text.setID(railID);
}
