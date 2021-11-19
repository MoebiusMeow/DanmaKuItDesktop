#include "KultLoginBox.h"
#include <QtWidgets>

KultLoginBox::KultLoginBox(QWidget *parent) : QGroupBox(parent)
{
    setupUI();
    switchToLogin();
    connect(this, &KultLoginBox::loginSuccess, this, &KultLoginBox::switchToDisplay);
}

void KultLoginBox::setupUI()
{
    QFont icons(QFontDatabase::applicationFontFamilies( QFontDatabase::addApplicationFont("://Assets/Fonts/fontawesome-webfont.ttf") ));
    icons.setFamily("FontAwesome");
    icons.setPixelSize(16);

    stackedLayout = new QStackedLayout();
    QGroupBox *group = new QGroupBox(this);
    QHBoxLayout *xLayout;
    QVBoxLayout *yLayout;
    QGridLayout *gLayout;
    QPushButton *tempButton;
    QLineEdit *tempLineEdit;
    QLabel *tempLabel;

    yLayout = new QVBoxLayout();
    yLayout->addStretch(1);

    xLayout = new QHBoxLayout();
    xLayout->addStretch(1);

    gLayout = new QGridLayout();
    tempLabel = new QLabel("房间号", group);
    gLayout->addWidget(tempLabel, 0, 0);
    tempLabel = new QLabel("房间密码", group);
    gLayout->addWidget(tempLabel, 1, 0);

    tempLineEdit = new QLineEdit(group);
    tempLineEdit->setMinimumWidth(220);
    tempLineEdit->setPlaceholderText("房间号");
    gLayout->addWidget(tempLineEdit, 0, 1);

    tempLineEdit = new QLineEdit(group);
    tempLineEdit->setMinimumWidth(220);
    tempLineEdit->setPlaceholderText("房间密码");
    tempLineEdit->setEchoMode(QLineEdit::Password);
    gLayout->addWidget(tempLineEdit, 1, 1);

    //xLayout->addLayout(gLayout);
    //xLayout->addStretch(1);
    //yLayout->addLayout(xLayout);
    //yLayout->addStretch(1);
    yLayout->addLayout(gLayout);
    yLayout->addStretch(1);

    //xLayout = new QHBoxLayout();
    //xLayout->addStretch(1);
    tempButton = new QPushButton("弹幕一下", group);
    tempButton->setMinimumWidth(200);
    connect(tempButton, &QPushButton::pressed, this, &KultLoginBox::login);
    yLayout->addWidget(tempButton);
    //xLayout->addStretch(1);

    xLayout->addLayout(yLayout);
    xLayout->addStretch(1);
    group->setLayout(xLayout);
    stackedLayout->addWidget(group);
    stackedLayout->addWidget(new QGroupBox(this));

    setLayout(stackedLayout);
}

void KultLoginBox::login()
{
    emit loginSuccess();
}


void KultLoginBox::switchToDisplay()
{
    stackedLayout->setCurrentIndex(1);
}

void KultLoginBox::switchToLogin()
{
    stackedLayout->setCurrentIndex(0);
}
