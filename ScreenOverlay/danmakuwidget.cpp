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

    setAttribute(Qt::WA_TransparentForMouseEvents);
}

DanmakuWidget::~DanmakuWidget()
{
    delete m_websocket;
}

int DanmakuWidget::appendFloat(const QString &text, const QString &id, const QColor &color, int size)
{
    std::shared_ptr<DanmakuTextFloat> newText = std::make_shared<DanmakuTextFloat>();
    newText->setText(text);
    newText->setColor(color);
    newText->setFontSize(size);
    newText->setID(id);
    m_textFloatSet->append(newText);
    //m_waiting.push_back(newText);
    return true;
}

int DanmakuWidget::appendTop(const QString &text, const QString &id, const QColor &color, int size)
{
    std::shared_ptr<DanmakuTextTop> newText = std::make_shared<DanmakuTextTop>();
    newText->setText(text);
    newText->setColor(color);
    newText->setFontSize(size);
    newText->setID(id);
    m_textTopSet->append(newText);
    return true;
}

int DanmakuWidget::appendBottom(const QString &text, const QString &id, const QColor &color, int size)
{
    std::shared_ptr<DanmakuTextBottom> newText = std::make_shared<DanmakuTextBottom>();
    newText->setText(text);
    newText->setColor(color);
    newText->setFontSize(size);
    newText->setID(id);
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

void DanmakuWidget::onJsonMessageRecieved(const QByteArray &message)
{
    QString str_message;
    str_message.prepend(message);
    str_message.replace('\'','\"');
    str_message.remove('\t');
    QByteArray processed_message = str_message.toUtf8();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(processed_message, &error);
    if(!doc.isNull()){
        QJsonObject json_obj = doc.object();
        QString message_type = json_obj.value("payload").toString();

        QJsonObject properties = json_obj.value("properties").toObject();
        if(properties.value("permission").toString().toInt()==DANMAKU_PERMISSION_ALLOW){
            QString content = properties.value("content").toString();
            QString id      = properties.value("id").toString();
            QString color_name = properties.value("color").toString();
            int     size    = properties.value("size").toString().split("pt", Qt::SkipEmptyParts)[0].toInt();
            QString pos     = properties.value("pos").toString();
            qDebug() << "content="<<content<<"  id="<<id<<" color="<<color_name<<"  size="<<size<<"  pos="<<pos;

            QColor color = QColor(0xFFFFFF);
            if(color_name != "undefined") color = QColor(color_name.toInt(NULL, 16));
            if(pos == "rightleft"){
                appendFloat(content, id, color, size);
            }
            if(pos == "top"){
                appendTop(content, id, color, size);
            }
            if(pos == "bottom"){
                appendBottom(content, id, color, size);
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
}
