#ifndef DANMAKUTEXTFLOAT_H
#define DANMAKUTEXTFLOAT_H

#include "DanmakuText.h"

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
