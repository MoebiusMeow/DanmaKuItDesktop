#include "KultLoginBox.h"
#include <QtWidgets>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "KultMessageBox.h"
#include "MainWindow.h"

KultLoginBox::KultLoginBox(NetworkAPI *network, QWidget *parent) :
    QGroupBox(parent), network(network), pMainWindow(parent)
{
    recentlyFailed = false;
    setupUI();
    switchToLogin();
    connect(this, &KultLoginBox::loginSuccess, this, &KultLoginBox::switchToDisplay);
    connect(this, &KultLoginBox::loginFailed, this, &KultLoginBox::handleLoginFailed);
    connect(this, &KultLoginBox::loginSuccess, this, &KultLoginBox::handleConnected);
    connect(network, &NetworkAPI::wsReconnectCountdown, this, &KultLoginBox::handleReconnecting);
}

void KultLoginBox::setupUI()
{
    QFont iconFont(QFontDatabase::applicationFontFamilies( QFontDatabase::addApplicationFont(":/Assets/Fonts/fontawesome-webfont.ttf") ));
    iconFont.setFamily("FontAwesome");
    iconFont.setPixelSize(16);

    stackedLayout = new QStackedLayout();
    QGroupBox *group;
    QGroupBox *titleGroup;
    QHBoxLayout *xLayout;
    QVBoxLayout *yLayout;
    QGridLayout *gLayout;
    QPushButton *tempButton;
    QLabel *tempLabel;

    // ----------
    // login
    // ----------
    group = new QGroupBox(this);

    yLayout = new QVBoxLayout();
    yLayout->addStretch(1);

    xLayout = new QHBoxLayout();
    xLayout->setContentsMargins(0, 0, 0, 0);

    tempButton = new QPushButton(QString(QChar(0xf013)), group);
    tempButton->setFont(iconFont);
    tempButton->setObjectName("SettingButton");
    tempButton->setToolTip(tr("设置房间服务器"));
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::switchToSetting);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::showSetting);
    xLayout->addWidget(tempButton);
    xLayout->setAlignment(tempButton, Qt::AlignBottom);

    xLayout->addStretch(1);

    gLayout = new QGridLayout();

    tempLabel = new QLabel(group);
    tempLabel->setText(QString(QChar(0xf015)));
    tempLabel->setFont(iconFont);
    tempLabel->setObjectName("IconLabel");
    gLayout->addWidget(tempLabel, 0, 0);
    //tempLabel = new QLabel("     房间号", group);
    //gLayout->addWidget(tempLabel, 0, 0);

    tempLabel = new QLabel(group);
    tempLabel->setText(QString(QChar(0xf084)));
    tempLabel->setFont(iconFont);
    tempLabel->setObjectName("IconLabel");
    gLayout->addWidget(tempLabel, 1, 0);

    loginHintLabel = tempLabel = new QLabel(group);
    tempLabel->setObjectName("LoginHintLabel");
    tempLabel->setText(tr("大写锁定已开启"));
    tempLabel->hide();
    gLayout->addWidget(tempLabel, 2, 1);

    roomidInput = new QLineEdit(group);
    roomidInput->setMinimumWidth(220);
    roomidInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[_a-zA-Z0-9\\+\\-\\*\\.\\@]+$"), roomidInput));
    roomidInput->setPlaceholderText(tr("房间号"));
    connect(roomidInput, &QLineEdit::returnPressed, this, &KultLoginBox::login);
    gLayout->addWidget(roomidInput, 0, 1);

    roompassInput = new QLineEdit(group);
    roompassInput->setMinimumWidth(220);
    roompassInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[_a-zA-Z0-9\\+\\-\\*\\.\\@]+$"), roompassInput));
    roompassInput->setPlaceholderText(tr("房间密码"));
    roompassInput->setEchoMode(QLineEdit::Password);
    roompassInput->installEventFilter(this);
    connect(roompassInput, &QLineEdit::returnPressed, this, &KultLoginBox::login);
    gLayout->addWidget(roompassInput, 1, 1);

    yLayout->addLayout(gLayout);
    yLayout->addStretch(1);

    tempButton = new QPushButton(tr("弹幕一下"), group);
    tempButton->setMinimumWidth(200);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::login);
    yLayout->addWidget(tempButton);

    xLayout->addLayout(yLayout);
    xLayout->addStretch(1);

    tempButton = new QPushButton(QString(QChar(0xf029)), group);
    tempButton->setFont(iconFont);
    tempButton->setObjectName("QRButton");
    tempButton->setToolTip(tr("二维码和弹幕墙"));
    // tempButton->setDisabled(true);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::showQRCode);
    xLayout->addWidget(tempButton);
    xLayout->setAlignment(tempButton, Qt::AlignBottom);

    group->setLayout(xLayout);
    stackedLayout->addWidget(group);

    // ----------
    // display
    // ----------

    group = new QGroupBox(this);
    group->setObjectName("DisplayBox");

    yLayout = new QVBoxLayout();
    yLayout->addStretch(1);

    xLayout = new QHBoxLayout();
    xLayout->setContentsMargins(0, 0, 0, 0);

    tempButton = new QPushButton(QString(QChar(0xf013)), group);
    tempButton->setFont(iconFont);
    tempButton->setObjectName("SettingButton");
    tempButton->setDisabled(true);
    xLayout->addWidget(tempButton);
    xLayout->setAlignment(tempButton, Qt::AlignBottom);

    xLayout->addStretch(1);


    gLayout = new QGridLayout();

    titleGroup = new QGroupBox(group);
    titleGroup->setLayout(new QHBoxLayout(titleGroup));
    titleGroup->setObjectName("TitleGroup");

    tempLabel = new QLabel(group);
    tempLabel->setText(QString(QChar(0xf058)));
    tempLabel->setFont(iconFont);
    titleGroup->layout()->addWidget(tempLabel);
    connectStatusLabel = tempLabel = new QLabel(tr("弹幕已连接"), group);
    titleGroup->layout()->addWidget(tempLabel);
    gLayout->addWidget(titleGroup, 0, 0, 1, 2);

    yLayout->addLayout(gLayout);
    yLayout->addStretch(1);

    tempButton = new QPushButton("断开连接", group);
    tempButton->setObjectName("DisconnectButton");
    tempButton->setMinimumWidth(200);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::logout);
    // connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::switchToLogin);
    // connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::backToLogin);
    yLayout->addWidget(tempButton);

    xLayout->addLayout(yLayout);
    xLayout->addStretch(1);

    tempButton = new QPushButton(QString(QChar(0xf029)), group);
    tempButton->setFont(iconFont);
    tempButton->setObjectName("QRButton");
    tempButton->setToolTip(tr("二维码和弹幕墙"));
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::showQRCode);
    xLayout->addWidget(tempButton);
    xLayout->setAlignment(tempButton, Qt::AlignBottom);

    group->setLayout(xLayout);
    stackedLayout->addWidget(group);

    // ----------
    // connecting
    // ----------

    group = new QGroupBox(this);

    yLayout = new QVBoxLayout();
    yLayout->addStretch(1);

    xLayout = new QHBoxLayout();
    xLayout->setContentsMargins(0, 0, 0, 0);
    xLayout->addStretch(1);

    gLayout = new QGridLayout();

    titleGroup = new QGroupBox(group);
    titleGroup->setLayout(new QHBoxLayout(titleGroup));
    titleGroup->layout()->setAlignment(Qt::AlignHCenter);
    titleGroup->setObjectName("TitleGroup");

    tempLabel = new QLabel(group);
    tempLabel->setText(QString(QChar(0xf254)));
    tempLabel->setFont(iconFont);
    titleGroup->layout()->addWidget(tempLabel);
    tempLabel = new QLabel(tr("正在连接房间服务器"), group);
    titleGroup->layout()->addWidget(tempLabel);
    gLayout->addWidget(titleGroup, 0, 0, 1, 2);

    yLayout->addLayout(gLayout);
    yLayout->addStretch(1);

    cancelConnectButton = tempButton = new QPushButton("取消连接", group);
    tempButton->setObjectName("CancelConnectButton");
    tempButton->setMinimumWidth(200);

    connect(tempButton, &QPushButton::pressed, network, &NetworkAPI::cancelConnect);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::switchToLogin);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::backToLogin);
    yLayout->addWidget(tempButton);

    xLayout->addLayout(yLayout);
    xLayout->addStretch(1);
    group->setLayout(xLayout);
    stackedLayout->addWidget(group);

    // ----------
    // login failed
    // ----------

    group = new QGroupBox(this);
    group->setObjectName("ErrorBox");

    yLayout = new QVBoxLayout();
    yLayout->addStretch(1);

    xLayout = new QHBoxLayout();
    xLayout->setContentsMargins(0, 0, 0, 0);
    xLayout->addStretch(1);

    gLayout = new QGridLayout();

    titleGroup = new QGroupBox(group);
    titleGroup->setLayout(new QHBoxLayout(titleGroup));
    titleGroup->layout()->setAlignment(Qt::AlignHCenter);
    titleGroup->setObjectName("TitleGroup");

    tempLabel = new QLabel(group);
    tempLabel->setText(QString(QChar(0xf071)));
    tempLabel->setFont(iconFont);
    titleGroup->layout()->addWidget(tempLabel);
    tempLabel = new QLabel(tr("连接失败"), group);
    titleGroup->layout()->addWidget(tempLabel);
    gLayout->addWidget(titleGroup, 0, 0, 1, 2);

    connectingLabel = tempLabel = new QLabel(tr("失败"), group);
    tempLabel->setAlignment(Qt::AlignHCenter);
    tempLabel->setWordWrap(true);
    tempLabel->setMaximumWidth(300);
    gLayout->addWidget(tempLabel, 1, 0, 1, 2);

    yLayout->addLayout(gLayout);
    yLayout->addStretch(1);

    tempButton = new QPushButton(tr("返回"), group);
    tempButton->setObjectName("CancelConnectButton");
    tempButton->setMinimumWidth(200);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::switchToLogin);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::backToLogin);
    yLayout->addWidget(tempButton);

    xLayout->addLayout(yLayout);
    xLayout->addStretch(1);
    group->setLayout(xLayout);
    stackedLayout->addWidget(group);


    // ----------
    // setting
    // ----------
    group = new QGroupBox(this);
    group->setObjectName("SettingBox");

    yLayout = new QVBoxLayout();
    yLayout->addStretch(1);

    xLayout = new QHBoxLayout();
    xLayout->setContentsMargins(0, 0, 0, 0);

    tempButton = new QPushButton(QString(QChar(0xf013)), group);
    tempButton->setFont(iconFont);
    tempButton->setObjectName("SettingButton");
    tempButton->setToolTip(tr("结束设置"));
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::switchToLogin);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::backToLogin);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::settingConfirm);
    xLayout->addWidget(tempButton);
    xLayout->setAlignment(tempButton, Qt::AlignBottom);

    xLayout->addStretch(1);

    gLayout = new QGridLayout();

    titleGroup = new QGroupBox(group);
    titleGroup->setLayout(new QHBoxLayout(titleGroup));
    titleGroup->layout()->setAlignment(Qt::AlignHCenter);
    titleGroup->setObjectName("TitleGroup");

    tempLabel = new QLabel(group);
    tempLabel->setText(QString(QChar(0xf015)));
    tempLabel->setFont(iconFont);
    tempLabel->setObjectName("IconLabel");
    gLayout->addWidget(tempLabel, 1, 0);

    tempLabel = new QLabel(group);
    tempLabel->setObjectName("SettingHintLabel");
    tempLabel->setText(tr("房间服务器域名（留空使用默认服务器）"));
    gLayout->addWidget(tempLabel, 2, 1);

    roomhostInput = new QLineEdit(group);
    roomhostInput->setMinimumWidth(270);
    // roomhostInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9_\\+\\-\\*\\.\\@]+$"), roomidInput));
    roomhostInput->setPlaceholderText(tr("房间服务器域名"));
    connect(roomhostInput, &QLineEdit::returnPressed, this, &KultLoginBox::login);
    gLayout->addWidget(roomhostInput, 1, 1);

    yLayout->addLayout(gLayout);
    yLayout->addStretch(1);

    tempButton = new QPushButton(tr("确认并返回"), group);
    tempButton->setMinimumWidth(200);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::switchToLogin);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::backToLogin);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::settingConfirm);
    yLayout->addWidget(tempButton);

    xLayout->addLayout(yLayout);
    xLayout->addStretch(1);

    tempButton = new QPushButton(QString(QChar(0xf029)), group);
    tempButton->setFont(iconFont);
    tempButton->setObjectName("QRButton");
    tempButton->setToolTip(tr("二维码和弹幕墙"));
    // tempButton->setDisabled(true);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::showQRCode);
    xLayout->addWidget(tempButton);
    xLayout->setAlignment(tempButton, Qt::AlignBottom);

    group->setLayout(xLayout);
    stackedLayout->addWidget(group);


    setLayout(stackedLayout);
}

void KultLoginBox::showQRCode()
{
    QString s = network->getWallUrl();
    if (s.isEmpty())
        KultMessageBox::information(pMainWindow, tr(""), tr("请先连接房间"), KultMessageBox::Confirm, KultMessageBox::NoButton);
    else
    {
        QUrl url(s);
        QDesktopServices::openUrl(url);
    }
}

void KultLoginBox::login()
{
    if (roomidInput->hasAcceptableInput() && roompassInput->hasAcceptableInput())
    {
        recentlyFailed = false;
        static_cast<MainWindow*>(pMainWindow)->setConfig("recent_room_id", roomidInput->text());
        // cancelConnectButton->setText(tr("取消连接"));
        switchToConnecting();
        m_id = roomidInput->text();
        QString pass = roompassInput->text();
        emit loginRequest(m_id, pass);
        emit connecting();
    }
    else
    {
        if (roomidInput->text().isEmpty() || roompassInput->text().isEmpty())
            KultMessageBox::information(pMainWindow, tr("非法输入"), tr("房间名/密码不能为空"), KultMessageBox::Confirm, KultMessageBox::NoButton);
        else
            KultMessageBox::information(pMainWindow, tr("非法输入"), tr("非法输入") + "\n\n" + tr("房间名/密码包含非法字符"), KultMessageBox::Confirm, KultMessageBox::NoButton);
    }
}


void KultLoginBox::logout()
{
    network->logout();
    switchToLogin();
    emit backToLogin();
}

void KultLoginBox::settingConfirm()
{
    static_cast<MainWindow*>(pMainWindow)->setConfig("recent_server", roomhostInput->text());
    static_cast<MainWindow*>(pMainWindow)->updateServer();
}

void KultLoginBox::switchToDisplay()
{
    stackedLayout->setCurrentIndex(1);
}

void KultLoginBox::switchToLogin()
{
    stackedLayout->setCurrentIndex(0);
}

void KultLoginBox::switchToConnecting()
{
    stackedLayout->setCurrentIndex(2);
}

void KultLoginBox::switchToLoginFailed()
{
    stackedLayout->setCurrentIndex(3);
}

void KultLoginBox::switchToSetting()
{
    stackedLayout->setCurrentIndex(4);
}

void KultLoginBox::handleLoginFailed(int errorType, QString errorMessage)
{
    if (recentlyFailed) return;
    recentlyFailed = true;
    connectingLabel->setText(errorMessage);
    switchToLoginFailed();
    // cancelConnectButton->setText(tr("返回"));
    if (errorType == -1)
        if (KultMessageBox::information(pMainWindow, tr("连接失败"), tr("连接失败\n\n\n") + errorMessage, KultMessageBox::Confirm, KultMessageBox::NoButton) == KultMessageBox::Confirm)
        {
            switchToLogin();
            emit backToLogin();
        }
}

void KultLoginBox::handleReconnecting(int countdown)
{
    connectStatusLabel->setText(tr("弹幕重连中") + "... " + QString::number(countdown));
}

void KultLoginBox::handleConnected()
{
    connectStatusLabel->setText(tr("弹幕已连接"));
}

bool KultLoginBox::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        //qDebug() << static_cast<QKeyEvent*>(event)->nativeModifiers();
        //qDebug() << static_cast<QKeyEvent*>(event)->modifiers();
        if (static_cast<QKeyEvent*>(event)->nativeModifiers() & 256)
            loginHintLabel->show();
        else
            loginHintLabel->hide();
    }
    if (event->type() == QEvent::FocusOut)
    {
        loginHintLabel->hide();
    }
    return QGroupBox::eventFilter(watched, event);
}
