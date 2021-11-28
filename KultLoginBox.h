#ifndef KULTLOGINBOX_H
#define KULTLOGINBOX_H

#include <QGroupBox>
#include <QObject>
#include <QtWidgets>

class KultLoginBox : public QGroupBox
{
    Q_OBJECT
signals:
    void loginSuccess();
    void logoutSuccess();

protected:
    QStackedLayout *stackedLayout;
    void setupUI();

public:
    KultLoginBox(QWidget *parent = nullptr);

public slots:
    void login();
    void logout();
    void switchToDisplay();
    void switchToLogin();
};

#endif // KULTLOGINBOX_H
