#include "danmakutextset.h"

#include <QDebug>
#include <QRandomGenerator>
#include <vector>

DanmakuTextSet::DanmakuTextSet(QObject *parent) : QObject(parent)
  ,m_texts(std::list<std::shared_ptr<DanmakuText>>())
{
}

bool DanmakuTextSet::append(std::shared_ptr<DanmakuText> text)
{
    m_waiting.push(text);
    return true;
}

bool DanmakuTextSet::setBound(const QRect &r)
{
    m_bound = r;
    calcRailYpos();
    return true;
}

int DanmakuTextSet::getRailCnt()
{
    return DANMAKU_RAIL_CNT;
}

int DanmakuTextSet::calcRailYpos()
{
    int deltY = m_bound.height()/(getRailCnt()+2);
    for(int i=0;i<getRailCnt();i++){
        m_railYpos[i] = deltY * (i+1) + m_bound.top();
    }
    return getRailCnt();
}

int DanmakuTextSet::popWaiting()
{
    if(!m_waiting.size()||!m_nAvailableRail) return 0;
    // text to pop
    std::shared_ptr<DanmakuText> text = m_waiting.front();
    m_waiting.pop();

    // select rail
    std::vector<int> available_rail;
    for(int i=0; i<getRailCnt(); i++)
        if(m_ifRailFree[i])
        {
            available_rail.push_back(i);
            if(available_rail.size()>=3) break;
        }
    int target_rail = available_rail[QRandomGenerator::global()->bounded(available_rail.size())];

    // send
    pushToRail(*text, target_rail);

    m_texts.push_back(text);

    return 0;
}

int DanmakuTextSet::updateRailStatus()
{
    // set all rail to free
    for(int i=0; i< getRailCnt(); i++){
        m_ifRailFree[i] = true;
    }

    for(auto i=m_texts.begin(); i!=m_texts.end(); i++){
        // find available rail
        if((*i)->id() < getRailCnt() && (*i)->id() >= 0 && ifBlockRail(*(*i), (*i)->id()))
            m_ifRailFree[(*i)->id()] = false;
    }

    // count available rail
    m_nAvailableRail = 0;
    for(int i=0;i<getRailCnt();i++) if(m_ifRailFree[i])m_nAvailableRail ++;
    return m_nAvailableRail;
}

bool DanmakuTextSet::ifBlockRail(const DanmakuText &text, int railID)
{
    return text.bound().right() >= m_bound.right() - 30 ;
    Q_UNUSED(railID);
}

void DanmakuTextSet::pushToRail(DanmakuText &text, int railID)
{
    text.setPos(QPointF(m_bound.right()-1, m_railYpos[railID]));
    text.setVel(QPointF(-2.5,0.0));
    text.setID(railID);
}

bool DanmakuTextSet::paint(QPainter *painter)
{
    painter->setRenderHints(QPainter::Antialiasing, true);
    for(auto i=m_texts.begin(); i!=m_texts.end(); i++){
        (*i)->calcBound(painter);
        (*i)->paint(painter);
    }
    return true;
}

bool DanmakuTextSet::update()
{
    // clear danmaku out of window
    m_texts.erase(remove_if(m_texts.begin(), m_texts.end(),
                           [this](const std::shared_ptr<DanmakuText> &i)
                            -> bool {return i->boundReady()&&!m_bound.intersects(i->bound());}
                           ),m_texts.end());

    // Update all danmaku and delete if return value of update() return false
    m_texts.erase(remove_if(m_texts.begin(), m_texts.end(),
                           [](const std::shared_ptr<DanmakuText> &i)
                            -> bool {return !i->update();}
                           ),m_texts.end());

    // update Rail Status
    updateRailStatus();

    // pop waiting danmaku
    popWaiting();

    return true;
}
