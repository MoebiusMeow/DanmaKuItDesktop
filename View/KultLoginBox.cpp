#include "KultLoginBox.h"
#include <QtWidgets>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "KultMessageBox.h"

KultLoginBox::KultLoginBox(QWidget *parent) :
    QGroupBox(parent), pMainWindow(parent)
{
    recentlyFailed = false;
    setupUI();
    switchToLogin();
    connect(this, &KultLoginBox::loginSuccess, this, &KultLoginBox::switchToDisplay);
    connect(this, &KultLoginBox::loginFailed, this, &KultLoginBox::handleLoginFailed);
}

void KultLoginBox::setupUI()
{
    QFont icons(QFontDatabase::applicationFontFamilies( QFontDatabase::addApplicationFont("://Assets/Fonts/fontawesome-webfont.ttf") ));
    icons.setFamily("FontAwesome");
    icons.setPixelSize(16);

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
    xLayout->addStretch(1);

    gLayout = new QGridLayout();

    tempLabel = new QLabel(group);
    tempLabel->setText(QString(QChar(0xf015)));
    tempLabel->setFont(icons);
    gLayout->addWidget(tempLabel, 0, 0);
    //tempLabel = new QLabel("     房间号", group);
    //gLayout->addWidget(tempLabel, 0, 0);

    tempLabel = new QLabel(group);
    tempLabel->setText(QString(QChar(0xf084)));
    tempLabel->setFont(icons);
    gLayout->addWidget(tempLabel, 1, 0);
    //tempLabel = new QLabel("     房间密码", group);
    //gLayout->addWidget(tempLabel, 1, 0);

    roomidInput = new QLineEdit(group);
    roomidInput->setMinimumWidth(220);
    roomidInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9\\+\\-\\*\\.\\@]+$"), roomidInput));
    roomidInput->setPlaceholderText(tr("房间号"));
    roomidInput->setText("1627286198");
    connect(roomidInput, &QLineEdit::returnPressed, this, &KultLoginBox::login);
    roomidInput->setText("3507228369");
    gLayout->addWidget(roomidInput, 0, 1);

    roompassInput = new QLineEdit(group);
    roompassInput->setMinimumWidth(220);
    roompassInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9\\+\\-\\*\\.\\@]+$"), roompassInput));
    roompassInput->setPlaceholderText(tr("房间密码"));
    roompassInput->setEchoMode(QLineEdit::Password);
    roompassInput->setText("hscizS");
    connect(roompassInput, &QLineEdit::returnPressed, this, &KultLoginBox::login);
    roompassInput->setText("YANtWT");
    gLayout->addWidget(roompassInput, 1, 1);

    yLayout->addLayout(gLayout);
    yLayout->addStretch(1);

    tempButton = new QPushButton(tr("弹幕一下"), group);
    tempButton->setMinimumWidth(200);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::login);
    yLayout->addWidget(tempButton);

    xLayout->addLayout(yLayout);
    xLayout->addStretch(1);
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
    xLayout->addStretch(1);

    gLayout = new QGridLayout();

    titleGroup = new QGroupBox(group);
    titleGroup->setLayout(new QHBoxLayout(titleGroup));
    titleGroup->setObjectName("TitleGroup");

    tempLabel = new QLabel(group);
    tempLabel->setText(QString(QChar(0xf058)));
    tempLabel->setFont(icons);
    titleGroup->layout()->addWidget(tempLabel);
    tempLabel = new QLabel(tr("弹幕已连接"), group);
    titleGroup->layout()->addWidget(tempLabel);
    gLayout->addWidget(titleGroup, 0, 0, 1, 2);

    yLayout->addLayout(gLayout);
    yLayout->addStretch(1);

    tempButton = new QPushButton("断开连接", group);
    tempButton->setObjectName("DisconnectButton");
    tempButton->setMinimumWidth(200);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::logout);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::switchToLogin);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::backToLogin);
    yLayout->addWidget(tempButton);

    xLayout->addLayout(yLayout);
    xLayout->addStretch(1);
    group->setLayout(xLayout);
    stackedLayout->addWidget(group);

    // ----------
    // connecting
    // ----------

    group = new QGroupBox(this);

    yLayout = new QVBoxLayout();
    yLayout->addStretch(1);

    xLayout = new QHBoxLayout();
    xLayout->addStretch(1);

    gLayout = new QGridLayout();

    titleGroup = new QGroupBox(group);
    titleGroup->setLayout(new QHBoxLayout(titleGroup));
    titleGroup->layout()->setAlignment(Qt::AlignHCenter);
    titleGroup->setObjectName("TitleGroup");

    tempLabel = new QLabel(group);
    tempLabel->setText(QString(QChar(0xf254)));
    tempLabel->setFont(icons);
    titleGroup->layout()->addWidget(tempLabel);
    tempLabel = new QLabel(tr("正在连接房间服务器"), group);
    titleGroup->layout()->addWidget(tempLabel);
    gLayout->addWidget(titleGroup, 0, 0, 1, 2);

    yLayout->addLayout(gLayout);
    yLayout->addStretch(1);

    cancelConnectButton = tempButton = new QPushButton("取消连接", group);
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
    // login failed
    // ----------

    group = new QGroupBox(this);
    group->setObjectName("ErrorBox");

    yLayout = new QVBoxLayout();
    yLayout->addStretch(1);

    xLayout = new QHBoxLayout();
    xLayout->addStretch(1);

    gLayout = new QGridLayout();

    titleGroup = new QGroupBox(group);
    titleGroup->setLayout(new QHBoxLayout(titleGroup));
    titleGroup->layout()->setAlignment(Qt::AlignHCenter);
    titleGroup->setObjectName("TitleGroup");

    tempLabel = new QLabel(group);
    tempLabel->setText(QString(QChar(0xf071)));
    tempLabel->setFont(icons);
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

    setLayout(stackedLayout);
}

void KultLoginBox::login()
{
    if (roomidInput->hasAcceptableInput() && roompassInput->hasAcceptableInput())
    {
        recentlyFailed = false;
        cancelConnectButton->setText(tr("取消连接"));
        switchToConnecting();
        m_id = roomidInput->text();
        QString pass = roompassInput->text();
        emit loginRequest(m_id, pass);
        emit connecting();
    }
    else
    {
        KultMessageBox::information(pMainWindow, tr("非法输入"), tr("非法输入") + "\n\n" + tr("房间名/密码包含非法字符"), KultMessageBox::Confirm, KultMessageBox::NoButton);
    }
}


void KultLoginBox::logout()
{
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

void KultLoginBox::onConnectionSuccess()
{
    switchToDisplay();
}
