#include "KultLoginBox.h"
#include <QtWidgets>

KultLoginBox::KultLoginBox(QWidget *parent) :
    QGroupBox(parent)
{
    setupUI();
    switchToLogin();
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
    roomidInput->setPlaceholderText("房间号");
    roomidInput->setText("3507228369");
    gLayout->addWidget(roomidInput, 0, 1);

    roompassInput = new QLineEdit(group);
    roompassInput->setMinimumWidth(220);
    roompassInput->setPlaceholderText("房间密码");
    roompassInput->setEchoMode(QLineEdit::Password);
    roompassInput->setText("YANtWT");
    gLayout->addWidget(roompassInput, 1, 1);

    yLayout->addLayout(gLayout);
    yLayout->addStretch(1);

    tempButton = new QPushButton("弹幕一下", group);
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
    tempLabel = new QLabel("弹幕已连接", group);
    titleGroup->layout()->addWidget(tempLabel);
    gLayout->addWidget(titleGroup, 0, 0, 1, 2);

    //tempLabel = new QLabel("房间号", group);
    //gLayout->addWidget(tempLabel, 1, 0);

    yLayout->addLayout(gLayout);
    yLayout->addStretch(1);

    tempButton = new QPushButton("断开连接", group);
    tempButton->setObjectName("DisconnectButton");
    tempButton->setMinimumWidth(200);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::logout);
    yLayout->addWidget(tempButton);

    xLayout->addLayout(yLayout);
    xLayout->addStretch(1);
    group->setLayout(xLayout);
    stackedLayout->addWidget(group);

    setLayout(stackedLayout);
}

void KultLoginBox::login()
{
    m_id = roomidInput->text();
    QString pass = roompassInput->text();
    emit loginRequest(m_id, pass);
}


void KultLoginBox::logout()
{
    switchToLogin();
}

void KultLoginBox::on_loginSuccess()
{
    switchToDisplay();
}

void KultLoginBox::on_loginFailed()
{
    return;
}

void KultLoginBox::switchToDisplay()
{
    stackedLayout->setCurrentIndex(1);
}

void KultLoginBox::switchToLogin()
{
    stackedLayout->setCurrentIndex(0);
}


