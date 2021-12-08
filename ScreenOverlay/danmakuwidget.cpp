#include "DanmakuWidget.h"

#include <QtDebug>
#include <QMouseEvent>
#include <QHostInfo>
#include <QFont>
#include <QFontDatabase>

#include "DanmakuTestSet.h"


DanmakuWidget::DanmakuWidget(QWidget *parent) : QWidget(parent)
    , m_textFloatSet(new DanmakuTextFloatSet(this))
    , m_textTopSet(new DanmakuTextTopSet(this))
    , m_textBottomSet(new DanmakuTextBottomSet(this))
    , update_timer(new QTimer(this))
    , reconnect_timer(new QTimer(this))
    , shutdown_timer(new QTimer(this))
    , m_test(new DanmakuTestSet(this))
    , m_websocket(new QWebSocket)
    , m_wsOn(false)
    , m_shutdown(false)
    , m_realshutdown(false)
{
    // config update_timer
    connect(update_timer, &QTimer::timeout, this, &DanmakuWidget::updateText);
    update_timer->start(UPDATE_INTERVAL);
    m_asyncRender.moveToThread(&m_renderThread);
    connect(this, &DanmakuWidget::asyncCreateText, &m_asyncRender, &DanmakuAsyncRender::renderText);
    m_renderThread.start();
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

DanmakuWidget::~DanmakuWidget()
{
    delete m_websocket;
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
    m_textFloatSet->paint(&p);
    m_textTopSet->paint(&p);
    m_textBottomSet->paint(&p);
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

void DanmakuWidget::onJsonMessageRecieved(const QJsonObject &json_obj)
{
    QString message_type = json_obj.value("payload").toString();

    QJsonObject properties = json_obj.value("properties").toObject();
    if(properties.value("permission").toString().toInt()==DANMAKU_PERMISSION_ALLOW) {
        QString content = properties.value("content").toString();
        QString id      = properties.value("id").toString();
        QString color_name = properties.value("color").toString();
        int     size    = properties.value("size").toString().split("pt", Qt::SkipEmptyParts)[0].toInt();
        QString pos     = properties.value("pos").toString();

        QColor color = QColor(0xFFFFFF);
        if(color_name != "undefined") color = QColor(color_name.toInt(NULL, 16));
        if(pos == "rightleft"){
            emit asyncCreateText(content, id, color, size, m_textFloatSet);
        }
        if(pos == "top"){
            emit asyncCreateText(content, id, color, size, m_textTopSet);
        }
        if(pos == "bottom"){
            emit asyncCreateText(content, id, color, size, m_textBottomSet);
        }
    }
    else if(message_type == "AAAB"){
        QString id      = properties.value("id").toString();
        std::shared_ptr<DanmakuText> p_text = nullptr;
        if(p_text == nullptr) p_text = m_textBottomSet->findByID(id);
        if(p_text == nullptr) p_text = m_textFloatSet->findByID(id);
        if(p_text == nullptr) p_text = m_textTopSet->findByID(id);

        if(p_text != nullptr) p_text->del();
    }
}

void DanmakuWidget::appendText(const QString &content, const QString id, const QColor &color, int size, int position)
{
    switch (position){
    case floatText:
        emit asyncCreateText(content, id, color, size, m_textFloatSet);
        break;
    case topText:
        emit asyncCreateText(content, id, color, size, m_textTopSet);
        break;
    case bottomText:
        emit asyncCreateText(content, id, color, size, m_textBottomSet);
        break;
    default:
        emit asyncCreateText(content, id, color, size, m_textFloatSet);
    }
}

DanmakuAsyncRender::DanmakuAsyncRender(QObject *parent) : QObject(parent)
{

}

void DanmakuAsyncRender::renderText(const QString &text, const QString &id, const QColor &color, int size, DanmakuTextSet *textset)
{
    qDebug()<<"1:"<<clock();
    std::shared_ptr<DanmakuTextFloat> newText = std::make_shared<DanmakuTextFloat>();
    newText->setText(text);
    newText->setColor(color);
    newText->setFontSize(size);
    newText->setID(id);
    qDebug()<<"c:"<<clock();
    newText->renderText();
    textset->append(newText);
}
