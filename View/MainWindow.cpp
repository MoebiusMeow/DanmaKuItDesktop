#include "MainWindow.h"
#include "KultMessageBox.h"
#include <QScreen>
#include <QLayout>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    draging = false;
    waveWidget = nullptr;
    screenOverlay = nullptr;
    setupUI();

    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    qreal ratio = screen()->devicePixelRatio();
    resize(screen()->geometry().width() * 0.25 * ratio, screen()->geometry().width() * 0.25 * 0.718 * ratio);
    move(screen()->geometry().center() - QPoint(width(), height()) / 2);

    installEventFilter(this);

    screenOverlay = new DanmakuWidget();
    screenOverlay->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    screenOverlay->setAttribute(Qt::WA_TranslucentBackground);
    screenOverlay->move(0, 0);
    screenOverlay->resize(screen()->geometry().width(), screen()->geometry().height());
    //screenOverlay->setWindowState(Qt::WindowMaximized);
    screenOverlay->hide();

    connect(loginBox, &KultLoginBox::wsConnectOK, screenOverlay, &DanmakuWidget::wsConnect);
    connect(screenOverlay, &DanmakuWidget::wsConnected, loginBox, &KultLoginBox::onConnectionSuccess);
}

void MainWindow::setupUI()
{
    // -----------
    // setup fonts
    // -----------

    QFont icons(QFontDatabase::applicationFontFamilies( QFontDatabase::addApplicationFont("://Assets/Fonts/fontawesome-webfont.ttf") ));
    icons.setFamily("FontAwesome");
    icons.setPixelSize(16);

    QStackedLayout *stretchFrame = new QStackedLayout(this);
    QHBoxLayout *xLayout;
    QVBoxLayout *yLayout;
    DynamicVBoxLayout *dyLayout;

    // -------------
    // overlay frame
    // -------------

    QFrame *overlayFrame = new QFrame(this);
    overlayFrame->setObjectName("MainOverlayFrame");
    //overlayFrame->setAttribute(Qt::WA_TransparentForMouseEvents);
    //overlayFrame->setAttribute(Qt::WA_TranslucentBackground);
    //overlayFrame->setDisabled(true);

    // yLayout = new QVBoxLayout(overlayFrame);
    dynamicLayout = dyLayout = new DynamicVBoxLayout(overlayFrame);

    dyLayout->setContentsMargins(0, 0, 0, 0);
    xLayout = new QHBoxLayout();
    xLayout->setContentsMargins(0, 0, 10, 0);
    xLayout->setSpacing(0);
    xLayout->addStretch(3);
    QPushButton *tempButton;
    tempButton = new QPushButton(this);
    // tempButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    tempButton->setText(QString(QChar(0xf068)));
    tempButton->setFont(icons);
    xLayout->addWidget(tempButton);
    connect(tempButton, &QPushButton::pressed, this, &MainWindow::handleMinimize);

    tempButton = new QPushButton(this);
    tempButton->setText(QString(QChar(0xf00d)));
    tempButton->setFont(icons);
    tempButton->setObjectName("WindowCloseButton");
    xLayout->addWidget(tempButton);
    connect(tempButton, &QPushButton::pressed, this, &MainWindow::handleClose);

    dyLayout->addLayout(xLayout);
    dyLayout->addStretch(3000);
    //QLabel *tempLabel;
    //tempLabel = new QLabel(this);
    //tempLabel->setObjectName("LogoFont");
    //tempLabel->setText(QString(QChar(0xf0e6)));
    //tempLabel->setAlignment(Qt::AlignCenter);
    //tempLabel->setFont(icons);
    //dyLayout->addWidget(tempLabel);

    //tempLabel = new QLabel(this);
    //tempLabel->setText("弹幕一下 Danmakult");
    //tempLabel->setAlignment(Qt::AlignCenter);
    //dyLayout->addWidget(tempLabel);

    loginBox = new KultLoginBox(this);
    loginBox->setObjectName("LoginBox");
    dyLayout->addWidget(loginBox, 1000);
    dyLayout->setTarget(dyLayout->count() - 1);
    dyLayout->animateStretch(8000, 1000);
    connect(loginBox, &KultLoginBox::loginSuccess, [this]{ dynamicLayout->animateStretch(5000, 600); waveWidget->animateTheme(1, 600); screenOverlay->show(); });
    connect(loginBox, &KultLoginBox::logoutSuccess, [this]{ dynamicLayout->animateStretch(8000, 600); waveWidget->animateTheme(0, 600); screenOverlay->hide(); });
    //yLayout->setStretch(1, 100);

    stretchFrame->addWidget(overlayFrame);
    //stretchFrame->addItem(yLayout);

    // ----------
    // base frame
    // ----------

    QFrame *frame = new QFrame(this);
    //frame->setFrameStyle(QFrame::Plain | QFrame::StyledPanel);
    frame->setObjectName("MainFrame");
    //frame->setAttribute(Qt::WA_TranslucentBackground, false);
    stretchFrame->addWidget(frame);
    stretchFrame->setStackingMode(QStackedLayout::StackAll);

    waveWidget = new WaveWidget(frame);
    waveWidget->setObjectName("MainFrameWaveView");
    waveWidget->setBorderRadiusLerp(500, 10);

    yLayout = new QVBoxLayout(frame);
    yLayout->setContentsMargins(0, 0, 0, 0);
    yLayout->addWidget(waveWidget, 3);
    yLayout->addStretch(2);

    QFile qssFile(":/Assets/Styles/mainUI.qss");
    qssFile.open(QFile::ReadOnly | QFile::Text);
    setStyleSheet(qssFile.readAll());
    qssFile.close();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (screenOverlay) screenOverlay->raise();
        draging = true;
        dragPosition = static_cast<QMouseEvent*>(event)->pos();
        return true;
    }
    if (event->type() == QEvent::MouseButtonRelease)
    {
        draging = false;
        return true;
    }
    if (draging && event->type() == QEvent::MouseMove)
    {
        move(static_cast<QMouseEvent*>(event)->pos() - (dragPosition + waveWidget->pos()) + pos());
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void MainWindow::handleMinimize()
{
    setWindowState(Qt::WindowMinimized);
}

void MainWindow::handleClose()
{
    if (KultMessageBox::information(this, "退出", "确定要退出吗\n（退出后将无法显示弹幕）", KultMessageBox::Confirm, KultMessageBox::Cancel) == KultMessageBox::Confirm)
    {
        screenOverlay->close();
        close();
    }
}

MainWindow::~MainWindow()
{
    if (waveWidget) { delete waveWidget; waveWidget = 0; }
}

