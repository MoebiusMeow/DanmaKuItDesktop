#include "danmakuwidget.h"

#include <QtDebug>
#include <QMouseEvent>

#include "danmakutestset.h"


DanmakuWidget::DanmakuWidget(QWidget *parent) : QWidget(parent)
    , m_textFloatSet(new DanmakuTextFloatSet)
    , m_textTopSet(new DanmakuTextTopSet)
    , m_textBottomSet(new DanmakuTextBottomSet)
    , update_timer(new QTimer)
    , paint_timer(new QTimer)
    , m_test(new DanmakuTestSet)
{
    connect(update_timer, &QTimer::timeout, this, &DanmakuWidget::updateText);
    update_timer->start(UPDATE_INTERVAL);
}

DanmakuWidget::~DanmakuWidget()
{
    delete m_textFloatSet;
    delete m_textTopSet;
    delete m_textBottomSet;
    delete update_timer;
    delete paint_timer;
    delete m_test;
}

int DanmakuWidget::append(const QString &text, const QColor &color)
{
    std::shared_ptr<DanmakuTextFloat> newText = std::make_shared<DanmakuTextFloat>();
    newText->setText(text);
    newText->setColor(color);
    m_textFloatSet->append(newText);
    //m_waiting.push_back(newText);
    return true;
}

int DanmakuWidget::appendTop(const QString &text, const QColor &color)
{
    std::shared_ptr<DanmakuTextTop> newText = std::make_shared<DanmakuTextTop>();
    newText->setText(text);
    newText->setColor(color);
    m_textTopSet->append(newText);
    return true;
}

int DanmakuWidget::appendBottom(const QString &text, const QColor &color)
{
    std::shared_ptr<DanmakuTextBottom> newText = std::make_shared<DanmakuTextBottom>();
    newText->setText(text);
    newText->setColor(color);
    m_textBottomSet->append(newText);
    return true;
}

bool DanmakuWidget::updateText()
{
    m_textFloatSet->update();
    m_textTopSet ->update();
    m_textBottomSet ->update();
    updateDisplay();
    return true;
}

bool DanmakuWidget::updateDisplay()
{
    update();
    return true;
}

void DanmakuWidget::paintEvent(QPaintEvent *paint_event)
{
    QPainter p(this);
    p.setFont(QFont("SimHei", 20, 1000));
    m_textFloatSet->paint(&p);
    m_textTopSet->paint(&p);
    m_textBottomSet->paint(&p);
    p.setBrush(Qt::blue);
    p.drawEllipse(QPoint(width()/2, height() * 0.6),50,50);
    raise();
    Q_UNUSED(paint_event);
}

void DanmakuWidget::resizeEvent(QResizeEvent *resize_event)
{
    m_textFloatSet->setBound(QRect(0,0,width(),height()));
    m_textTopSet->setBound(QRect(0,0,width(),height()));
    m_textBottomSet->setBound(QRect(0,0,width(),height()));
    Q_UNUSED(resize_event);
}

void DanmakuWidget::mousePressEvent(QMouseEvent *mouse_event)
{
    if(mouse_event->button()==Qt::LeftButton){
        append(m_test->getRandonStr());
    }
    if(mouse_event->button()==Qt::RightButton){
        appendTop(m_test->getRandonStr());
    }
    if(mouse_event->button()==Qt::MiddleButton){
        appendBottom(m_test->getRandonStr());
    }
    Q_UNUSED(mouse_event)
}
