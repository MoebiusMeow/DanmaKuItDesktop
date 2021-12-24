#include "DanmakuTextFloatSet.h"

DanmakuTextFloatSet::DanmakuTextFloatSet(QObject *parent) : DanmakuTextSet(parent)
{

}

int DanmakuTextFloatSet::getRailCnt()
{
    return DANMAKU_FLOAT_RAIL_CNT;
}
