#ifndef KULTLOGINBOX_H
#define KULTLOGINBOX_H

#include <QGroupBox>
#include <QObject>
#include <QtWidgets>


class KultLoginBox : public QGroupBox
{
    Q_OBJECT

signals:
    void loginRequest(QString roomID, QString password);
    void logoutRequest();

protected:
    QStackedLayout *stackedLayout;
    QLineEdit *roomidInput, *roompassInput;
    void setupUI();
    QString m_id;// room id

public:
    KultLoginBox(QWidget *parent = nullptr);

protected slots:
    void login();
    void logout();
    void switchToDisplay();
    void switchToLogin();

public slots:
    void on_loginSuccess();
    void on_loginFailed();

};

#endif // KULTLOGINBOX_H
