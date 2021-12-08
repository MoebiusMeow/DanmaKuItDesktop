#ifndef DANMAKUTEXTBOTTOMSET_H
#define DANMAKUTEXTBOTTOMSET_H

#include "DanmakuTextBottom.h"
#include "DanmakuTextSet.h"

const int DANMAKU_BOTTOM_RAIL_CNT = 8;

class DanmakuTextBottomSet : public DanmakuTextSet
{
    Q_OBJECT
public:
    explicit DanmakuTextBottomSet(QObject *parent = nullptr);

protected:
    int  getRailCnt() override;
    int  calcRailYpos() override;
    bool ifBlockRail(const DanmakuText &text, int railID) override;
    void pushToRail(DanmakuText &text, int railID) override;
};

#endif // DANMAKUTEXTBOTTOMSET_H
