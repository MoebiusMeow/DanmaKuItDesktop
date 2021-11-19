#ifndef DANMAKUWIDGET_H
#define DANMAKUWIDGET_H

#include <QWidget>
#include <QTimer>
#include "danmakutextfloatset.h"
#include "danmakutexttopset.h"
#include "danmakutextbottomset.h"

class DanmakuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DanmakuWidget(QWidget *parent = nullptr);
    ~DanmakuWidget();

    const int UPDATE_INTERVAL = 20;
    const int PAINT_INTERVAL  = 25;

    int append(const QString &text, const QColor &color = Qt::cyan);
    int appendTop(const QString &text, const QColor &color = Qt::magenta);
    int appendBottom(const QString &text, const QColor &color = Qt::yellow);

protected slots:
    bool updateText();
    bool updateDisplay();
    void paintEvent(QPaintEvent *paint_event) override;
    void resizeEvent(QResizeEvent *resize_event) override;
    void mousePressEvent(QMouseEvent *mouse_event) override;

private:
    DanmakuTextFloatSet *m_textFloatSet;
    DanmakuTextTopSet *m_textTopSet;
    DanmakuTextBottomSet *m_textBottomSet;
    QTimer *update_timer, *paint_timer;
};

#endif // DANMAKUWIDGET_H
