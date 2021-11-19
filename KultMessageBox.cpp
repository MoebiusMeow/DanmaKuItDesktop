#include "KultMessageBox.h"

KultMessageBox::KultMessageBox(QWidget *parent) : QDialog(parent)
{
    draging = false;
    button1 = nullptr;
    button2 = nullptr;
    label = new QLabel(this);
    frame = new QFrame(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    installEventFilter(this);
}

void KultMessageBox::setupUI()
{
    QVBoxLayout *stretchFrame = new QVBoxLayout(this);
    stretchFrame->addWidget(frame);

    QVBoxLayout *yLayout = new QVBoxLayout(frame);
    yLayout->addWidget(label, 3);
    QHBoxLayout *xLayout = new QHBoxLayout();
    if (button1) xLayout->addWidget(button1);
    if (button2) xLayout->addWidget(button2);
    yLayout->addLayout(xLayout);
}

void KultMessageBox::setText(QString text)
{
    label->setText(text);
}

void KultMessageBox::closeEvent(QCloseEvent *event)
{
    setResult(NoButton);
    QDialog::closeEvent(event);
}

void KultMessageBox::setupButtonState(QPushButton *button, KultMessageBoxButton type)
{
    switch (type)
    {
        case Confirm:
            button->setText("确认");
            break;
        case Cancel:
            button->setText("取消");
            break;
        default:
            break;
    }
}

KultMessageBox::KultMessageBoxButton KultMessageBox::information(QWidget *parent, QString title, QString text, KultMessageBoxButton button1, KultMessageBoxButton button2)
{
    KultMessageBox *box = new KultMessageBox(parent);
    box->setWindowTitle(title);
    box->setText(text);
    if (button1 != NoButton)
    {
       box->button1 = new QPushButton(box);
       setupButtonState(box->button1, button1);
       connect(box->button1, &QPushButton::pressed, std::bind(&KultMessageBox::done, box, (int)button1));
    }
    if (button2 != NoButton)
    {
       box->button2 = new QPushButton(box);
       setupButtonState(box->button2, button2);
       connect(box->button2, &QPushButton::pressed, std::bind(&KultMessageBox::done, box, (int)button2));
    }
    box->setupUI();
    return KultMessageBoxButton(box->exec());
    return KultMessageBoxButton::NoButton;
}

bool KultMessageBox::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
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
        move(static_cast<QMouseEvent*>(event)->pos() - dragPosition + pos());
        return true;
    }
    return QDialog::eventFilter(watched, event);
}

KultMessageBox::~KultMessageBox()
{
}
