#include "DanmakuTestSet.h"

#include <QRandomGenerator>

DanmakuTestSet::DanmakuTestSet(QObject *parent) : QObject(parent)
{

}

QString DanmakuTestSet::getRandonStr()
{
    return danmaku_test_data[ QRandomGenerator::global()->bounded(84)];
}
