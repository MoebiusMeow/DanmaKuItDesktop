#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "WaveWidget.h"
#include "ScreenOverlay/danmakuwidget.h"
#include "DynamicVBoxLayout.h"
#include "KultLoginBox.h"
#include <QWidget>

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    bool draging;
    QPoint dragPosition;

protected:
    WaveWidget *waveWidget;
    DanmakuWidget *screenOverlay;
    DynamicVBoxLayout *dynamicLayout;
    KultLoginBox *loginBox;

    void setupUI();
    bool eventFilter(QObject *watched, QEvent *event) override;

protected slots:
    void handleMinimize();
    void handleClose();

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
