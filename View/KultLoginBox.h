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
    void wsConnectOK(const QString &roomid, const QString &token);
    void loginFailed(QString errorMessage);
    void connecting();
    void backToLogin();
    void loginRequest(QString roomID, QString password);
    void logoutRequest();

protected:
    bool recentlyFailed;
    QWidget *pMainWindow;
    QStackedLayout *stackedLayout;
    QLineEdit *roomidInput, *roompassInput;
    QLabel *connectingLabel;
    QString m_id;// room id
    void setupUI();

public:
    KultLoginBox(QWidget *parent = nullptr);

protected slots:
    void login();
    void logout();
    void switchToDisplay();
    void switchToConnecting();
    void switchToLogin();

    void handleLoginFailed(QString errorMessage);
    void onConnectionSuccess();
};

#endif // KULTLOGINBOX_H
