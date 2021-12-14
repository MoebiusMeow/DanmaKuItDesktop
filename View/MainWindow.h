#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "WaveWidget.h"
#include "ScreenOverlay/DanmakuWidget.h"
#include "Network/NetworkAPI.h"
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
    QSystemTrayIcon *systemTrayIcon;
    NetworkAPI *network;
    QJsonDocument configDocument;

    void setupUI();
    void loadConfig();
    void saveConfig();
    bool eventFilter(QObject *watched, QEvent *event) override;

protected slots:
    void handleMinimize();
    void handleClose();
    void handleShow();

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setConfig(QString key, QJsonValue value);
    void updateServer();
};
#endif // MAINWINDOW_H
