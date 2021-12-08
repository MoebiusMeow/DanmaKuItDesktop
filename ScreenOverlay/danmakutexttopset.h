#ifndef DANMAKUTEXTTOPSET_H
#define DANMAKUTEXTTOPSET_H

#include "DanmakuTextSet.h"
#include "DanmakuTextTop.h"

const int DANMAKU_TOP_RAIL_CNT = 8;

class DanmakuTextTopSet : public DanmakuTextSet
{
    Q_OBJECT
public:
    explicit DanmakuTextTopSet(QObject *parent = nullptr);

protected:
    int  getRailCnt() override;
    int  calcRailYpos() override;
    bool ifBlockRail(const DanmakuText &text, int railID) override;
    void pushToRail(DanmakuText &text, int railID) override;
};

#endif // DANMAKUTEXTTOPSET_H
