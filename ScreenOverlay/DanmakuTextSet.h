#ifndef DANMAKUTEXTSET_H
#define DANMAKUTEXTSET_H

#include <QObject>
#include <list>
#include <queue>

#include "DanmakuText.h"
#include "DanmakuConstants.h"

const int DANMAKU_TEXT_INTERVAL = 30;
const int DANMAKU_MAX_RAIL_CNT = 200;

class DanmakuTextSet : public QObject
{
    Q_OBJECT
public:
    explicit DanmakuTextSet(QObject *parent = nullptr);
    bool setBound(const QRect &r);
    std::shared_ptr<DanmakuText> findByID(const QString &id);
public Q_SLOTS:
    /* Use this to append new danmaku*/
    bool append(std::shared_ptr<DanmakuText> text);

protected:
    virtual int getRailCnt() = 0; // NOTE: do not exceed DANMAKU_MAX_RAIL_CNT
    virtual int calcRailYpos(); // NOTE: Will be called when created. Override this to modify rail position
    virtual int popWaiting();
    virtual int updateRailStatus();// NOTE: This will use ifBlockRail() to judge
    virtual bool ifBlockRail(const DanmakuText &text,int railID); // NOTE: Override this if you change the way danmaku moves
    virtual void pushToRail(DanmakuText &text, int railID); // NOTE: Set attribute of the danmaku here

protected:
    std::list<std::shared_ptr<DanmakuText>> m_texts;
    QRect m_bound;

    std::list<std::shared_ptr<DanmakuText>> m_waiting;
    int m_railYpos[DANMAKU_MAX_RAIL_CNT];
    bool m_ifRailFree[DANMAKU_MAX_RAIL_CNT];
    int m_nAvailableRail;


public:
    bool paint(QPainter *painter);
    bool update();

signals:

};

#endif // DANMAKUTEXTSET_H
