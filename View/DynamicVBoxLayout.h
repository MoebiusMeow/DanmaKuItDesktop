#ifndef DYNAMICVBOXLAYOUT_H
#define DYNAMICVBOXLAYOUT_H

#include <QVBoxLayout>
#include <QObject>
#include <QtWidgets>

class DynamicVBoxLayout : public QVBoxLayout
{
    Q_OBJECT
    Q_PROPERTY(int targetStretch READ getTargetStretch WRITE setTargetStretch)

private:
    int target;

public:
    void setTarget(int targetIndex);
    int getTargetStretch();
    void setTargetStretch(int value);
    void animateStretch(int endValue, int duration = 1000);

    DynamicVBoxLayout(QWidget *parent = nullptr);
};

#endif // DYNAMICVBOXLAYOUT_H
