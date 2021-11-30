#include "danmakuwidget.h"

#include <QtDebug>
#include <QMouseEvent>

#include "danmakutestset.h"


DanmakuWidget::DanmakuWidget(QWidget *parent) : QWidget(parent)
    , m_textFloatSet(new DanmakuTextFloatSet)
    , m_textTopSet(new DanmakuTextTopSet)
    , m_textBottomSet(new DanmakuTextBottomSet)
    , update_timer(new QTimer)
    , reconnect_timer(new QTimer)
    , m_test(new DanmakuTestSet)
    , m_websocket(new QWebSocket)
{
    // config websocket
    m_url = "ws://127.0.0.1:8000";
    connect(m_websocket, &QWebSocket::connected, this, &DanmakuWidget::onconnect);
    connect(m_websocket, &QWebSocket::disconnected, this, &DanmakuWidget::ondisconnect);
    connect(m_websocket, &QWebSocket::binaryMessageReceived, this, &DanmakuWidget::onBinaryMessageRecieved);
    connect(m_websocket, &QWebSocket::textMessageReceived, this, &DanmakuWidget::onTextMessageReceived);
    m_websocket->open(m_url);
    qDebug()<<"conncting...";

    // config update_timer
    connect(update_timer, &QTimer::timeout, this, &DanmakuWidget::updateText);
    update_timer->start(UPDATE_INTERVAL);
}

DanmakuWidget::~DanmakuWidget()
{
    m_websocket->close();
    delete m_textFloatSet;
    delete m_textTopSet;
    delete m_textBottomSet;
    delete update_timer;
    delete reconnect_timer;
    delete m_test;
    delete m_websocket;
}

int DanmakuWidget::appendFloat(const QString &text, const QString &id, const QColor &color, int size)
{
    std::shared_ptr<DanmakuTextFloat> newText = std::make_shared<DanmakuTextFloat>();
    newText->setText(text);
    newText->setColor(color);
    newText->setFontSize(size);
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
    m_textTopSet->append(newText);
    return true;
}

int DanmakuWidget::appendBottom(const QString &text, const QString &id, const QColor &color, int size)
{
    std::shared_ptr<DanmakuTextBottom> newText = std::make_shared<DanmakuTextBottom>();
    newText->setText(text);
    newText->setColor(color);
    newText->setFontSize(size);
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

void DanmakuWidget::onconnect()
{
    qDebug()<<"connnected to room server";
}

void DanmakuWidget::ondisconnect()
{
    //reconnect_timer->singleShot(DANMAKU_RECONNECT_INTERVAL,this,&DanmakuWidget::reconnect);
}

void DanmakuWidget::reconnect()
{
    m_websocket->abort();
    m_websocket->open(m_url);
}

void DanmakuWidget::onBinaryMessageRecieved(const QByteArray &message)
{
    //
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

void DanmakuWidget::onTextMessageReceived(const QString &message)
{
    qDebug()<<"[Text]"<<message;
}
