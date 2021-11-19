#ifndef DANMAKUTEXTFLOATSET_H
#define DANMAKUTEXTFLOATSET_H

#include "danmakutextfloat.h"
#include "danmakutextset.h"

class DanmakuTextFloatSet : public DanmakuTextSet
{
    Q_OBJECT
public:
    explicit DanmakuTextFloatSet(QObject *parent = nullptr);
};

#endif // DANMAKUTEXTFLOATSET_H
