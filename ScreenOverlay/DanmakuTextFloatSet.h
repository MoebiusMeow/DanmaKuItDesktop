#ifndef DANMAKUTEXTFLOATSET_H
#define DANMAKUTEXTFLOATSET_H

#include "DanmakuTextFloat.h"
#include "DanmakuTextSet.h"

class DanmakuTextFloatSet : public DanmakuTextSet
{
    Q_OBJECT
public:
    explicit DanmakuTextFloatSet(QObject *parent = nullptr);
    int getRailCnt() override;
};

#endif // DANMAKUTEXTFLOATSET_H
