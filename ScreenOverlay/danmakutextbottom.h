#ifndef DANMAKUTEXTBOTTOM_H
#define DANMAKUTEXTBOTTOM_H

#include "danmakutext.h"

class DanmakuTextBottom : public DanmakuText
{
    Q_OBJECT
public:
    explicit DanmakuTextBottom(QObject *parent = nullptr);

public:
    bool setLife(int life);

protected:
    bool update() override;

    bool paint(QPainter *painter) override;
    void calcBound(QPainter *painter) override;

private:
    int m_life;
};

#endif // DANMAKUTEXTBOTTOM_H
