#ifndef KULTLOGINBOX_H
#define KULTLOGINBOX_H

#include <QGroupBox>
#include <QObject>
#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <NetworkConfig.h>

class KultLoginBox : public QGroupBox
{
    Q_OBJECT
signals:
    void loginSuccess();
    void logoutSuccess();
    void wsConnectOK(const QString &roomid, const QString &token);
    void loginFailed();

protected:
    QStackedLayout *stackedLayout;
    QLineEdit *roomidInput, *roompassInput;
    void setupUI();
    QNetworkAccessManager *netManager;
    QNetworkReply *m_reply;
    QString m_id;// room id

protected:
    bool loginWithIDPass(const QString& id, const QString &pass );

public:
    KultLoginBox(QWidget *parent = nullptr);

public slots:
    void login();
    void logout();
    void switchToDisplay();
    void switchToLogin();
    void handleLoginReply();
    void onConnectionSuccess();
};

#endif // KULTLOGINBOX_H
