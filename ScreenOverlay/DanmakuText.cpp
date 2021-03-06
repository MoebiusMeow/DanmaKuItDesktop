#include "DanmakuText.h"

#include <QtDebug>
#include <QPainterPath>
#include <QFont>
#include <QFontDatabase>
#include <QGuiApplication>

bool DanmakuText::s_font_loaded = false;
QFont DanmakuText::s_default_font;

DanmakuText::DanmakuText(QObject *parent) : QObject(parent)
  ,m_pos(QPointF(0.0,0.0))
  ,m_vel(QPointF(0.0,0.0))
  ,m_text(QString("EmptyText"))
  ,m_color(QColor(0xff,0xff,0xff))
  ,m_font_size(16)
  ,m_boundReady(false)
  ,m_bufferImageReady(false)
  ,m_delTag(false)
{
    if (!DanmakuText::s_font_loaded)
    {
        QStringList families;
        families.append(QFontDatabase::applicationFontFamilies( QFontDatabase::addApplicationFont(":/Assets/Fonts/NotoSansCJKsc-Bold.otf") ));
        families.append(QFontDatabase::applicationFontFamilies( QFontDatabase::addApplicationFont(":/Assets/Fonts/NotoColorEmoji_WindowsCompatible.ttf") ));

        DanmakuText::s_default_font = QFont(families);
        // windows emoji
        DanmakuText::s_default_font.setFamilies(families);
        DanmakuText::s_font_loaded = true;
    }
    m_font = DanmakuText::s_default_font;
    m_font.setPointSize(m_font_size);
    m_bufferImage = nullptr;
}

DanmakuText::~DanmakuText()
{
    if(m_bufferImage){
        m_bufferImage->detach();
        delete m_bufferImage;
    }
}

void DanmakuText::renderText()
{
    QFontMetrics m(m_font);
    calcBound();
    int py = (m_bound.height() - m.height()) + m.ascent() - (m.boundingRect(m_text).bottom());

    QPainterPath path;
    path.addText(5, py, m_font, m_text);
    m_bufferImage = new QImage(m_bound.width(), m_bound.height(), QImage::Format_ARGB32);
    m_bufferImage->fill(QColor(0,0,0,0));
    auto *bufferPainter = new QPainter(m_bufferImage);
    bufferPainter->setRenderHints(QPainter::Antialiasing, true);

    bufferPainter->strokePath(path, QPen((m_color.valueF()<0.5) ? Qt::white : Qt::black, 3, Qt::SolidLine));
    bufferPainter->fillPath(path, QBrush(m_color));
    bufferPainter->setPen(QColor(0,0,0,0));
    bufferPainter->setRenderHint(QPainter::RenderHint::TextAntialiasing, false);

    bufferPainter->setFont(m_font);
    bufferPainter->drawText(5, py, m_text);
    delete(bufferPainter);
    m_bufferImageReady = true;
}

bool DanmakuText::setText(const QString &text)
{
    m_text = text;
    m_boundReady = false;
    return true;
}

bool DanmakuText::setPos(const QPointF &pos)
{
    m_pos = pos;
    m_boundReady = false;
    return true;
}

bool DanmakuText::setVel(const QPointF &vel)
{
    m_vel = vel;
    return true;
}

bool DanmakuText::setColor(const QColor &color)
{
    m_color = color;
    return true;
}

bool DanmakuText::setFontSize(int size)
{
    m_font_size = size;
    m_font.setPointSize(size);
    return true;
}

bool DanmakuText::setID(const QString &id)
{
    m_id = id;
    return true;
}

bool DanmakuText::setRailID(int railid)
{
    m_railid = railid;
    return true;
}

void DanmakuText::del()
{
    m_delTag = true;
}

QPoint DanmakuText::pos() const
{
    return QPoint(m_pos.x(), m_pos.y());
}

QPointF DanmakuText::posF() const
{
    return m_pos;
}

QRect DanmakuText::bound() const
{
    return m_bound;
}

bool DanmakuText::boundReady() const
{
    return m_boundReady;
}

QString DanmakuText::id() const
{
    return m_id;
}

int DanmakuText::railID() const
{
    return m_railid;
}

bool DanmakuText::paint(QPainter *painter)
{
    if(!m_bufferImageReady){
        renderText();
    }
    painter->setRenderHints(QPainter::SmoothPixmapTransform, true);
    painter->drawImage(m_bound, *m_bufferImage);
    // painter->setPen(Qt::white);
    // painter->drawRect(m_bound);
    return true;
}

bool DanmakuText::update()
{
    if(!m_timer_started){
        m_timer_started = true;
        m_timer.start();
        return true;
    }
    m_timer.restart();
    if(m_delTag) return false;
    return true;
}

void DanmakuText::calcBound()
{
    if(!m_boundReady){
        m_bound = QFontMetrics(m_font).boundingRect(m_text);
        m_bound.adjust(-5, -5, 5, 5);
    }
    m_boundReady = true;
}

clock_t DanmakuText::getFrameInterval()
{
    return m_timer_started? (m_timer.elapsed())/(1000/DANMAKU_FPS) :0;
}
