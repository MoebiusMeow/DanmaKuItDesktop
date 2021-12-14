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
    void loginFailed(int errorType, QString errorMessage);
    void connecting();
    void backToLogin();
    void loginRequest(QString roomID, QString password);
    void logoutRequest();
    void showSetting();

protected:
    bool recentlyFailed;
    QWidget *pMainWindow;
    QStackedLayout *stackedLayout;
    QLineEdit *roompassInput;
    QLabel *loginHintLabel;
    QLabel *connectingLabel;
    QPushButton *cancelConnectButton;
    QString m_id; // room id
    void setupUI();
    bool eventFilter(QObject *watched, QEvent *event) override;

public:
    QLineEdit *roomidInput, *roomhostInput;
    KultLoginBox(QWidget *parent = nullptr);

protected slots:
    void login();
    void logout();
    void switchToDisplay();
    void switchToConnecting();
    void switchToLogin();
    void switchToLoginFailed();
    void switchToSetting();

    void settingConfirm();
    void showQRCode();

    void handleLoginFailed(int errorType, QString errorMessage);
};

#endif // KULTLOGINBOX_H
