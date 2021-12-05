#ifndef DANMAKUTEXTFLOAT_H
#define DANMAKUTEXTFLOAT_H

#include "danmakutext.h"

class DanmakuTextFloat : public DanmakuText
{
    Q_OBJECT
public:
    explicit DanmakuTextFloat(QObject *parent = nullptr);
public:
    bool update() override;

    void calcBound(QPainter *painter) override;

};

#endif // DANMAKUTEXTFLOAT_H
