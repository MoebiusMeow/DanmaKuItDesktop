#include "DynamicVBoxLayout.h"

DynamicVBoxLayout::DynamicVBoxLayout(QWidget *parent) : QVBoxLayout(parent)
{
}

void DynamicVBoxLayout::setTarget(int targetIndex)
{
    target = targetIndex;
}

void DynamicVBoxLayout::setTargetStretch(int value)
{
    this->setStretch(target, value);
}

int DynamicVBoxLayout::getTargetStretch()
{
    return this->stretch(target);
}

void DynamicVBoxLayout::animateStretch(int endValue, int duration)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "targetStretch", this);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setEndValue(endValue);
    animation->setDuration(duration);
    animation->start();
}
