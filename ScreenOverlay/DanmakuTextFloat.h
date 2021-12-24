#ifndef DANMAKUTEXTFLOAT_H
#define DANMAKUTEXTFLOAT_H

#include "DanmakuText.h"

#define DANMAKU_FLOAT_RAIL_CNT = 14;

class DanmakuTextFloat : public DanmakuText
{
    Q_OBJECT
public:
    explicit DanmakuTextFloat(QObject *parent = nullptr);
public:
    bool update() override;

    void calcBound() override;

};

#endif // DANMAKUTEXTFLOAT_H
