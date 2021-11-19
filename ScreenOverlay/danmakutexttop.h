#ifndef DANMAKUTEXTTOP_H
#define DANMAKUTEXTTOP_H

#include "danmakutext.h"



class DanmakuTextTop : public DanmakuText
{
    Q_OBJECT
public:
    explicit DanmakuTextTop(QObject *parent = nullptr);

public:
    bool setLife(int life);

protected:
    bool update() override;

    bool paint(QPainter *painter) override;
    void calcBound(QPainter *painter) override;

private:
    int m_life;
};

#endif // DANMAKUTEXTTOP_H
