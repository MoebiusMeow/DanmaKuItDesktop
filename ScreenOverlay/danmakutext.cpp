#include "danmakutext.h"

#include <QtDebug>
#include <QPainterPath>

DanmakuText::DanmakuText(QObject *parent) : QObject(parent)
  ,m_pos(QPointF(0.0,0.0))
  ,m_vel(QPointF(0.0,0.0))
  ,m_text(QString("123"))
  ,m_color(QColor(0xff,0xff,0xff))
  ,m_font_size(16)
  ,m_last_clock(0)
  ,m_boundReady(false)
  ,m_bufferImageReady(false)
  ,m_delTag(false)
{

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
    return true;
}

bool DanmakuText::setID(int id)
{
    m_id = id;
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

int DanmakuText::id() const
{
    return m_id;
}

bool DanmakuText::paint(QPainter *painter)
{
    const QFontMetrics &m = painter->fontMetrics();
    int py = (m_bound.height()-m.height()) + m.ascent();

    if(!m_bufferImageReady){
        m_bufferImage = std::make_shared<QImage>(m_bound.width(),m_bound.height(),QImage::Format_ARGB32);
        m_bufferImage->fill(QColor(0,0,0,0));
        QPainter *bufferPainter = new QPainter(m_bufferImage.get());
        bufferPainter->setFont(QFont(DANMAKU_DEFAULT_FONT, m_font_size, DANMAKU_DEFAULT_TEXT_WEIGHT));
        bufferPainter->setRenderHints(QPainter::Antialiasing, true);
        QPainterPath path;
        path.addText(0, py, bufferPainter->font(), m_text);
        bufferPainter->setBrush(m_color);
        bufferPainter->setPen(QPen((m_color.valueF()<0.5) ? Qt::white : Qt::black,2,Qt::SolidLine));
        bufferPainter->drawPath(path);
        bufferPainter->fillPath(path,QBrush(m_color));
        delete(bufferPainter);
        m_bufferImageReady = true;
    }

    painter->setRenderHints(QPainter::SmoothPixmapTransform, true);
    painter->drawImage(m_bound,*m_bufferImage);
    // painter->setPen(Qt::white);
    // painter->drawRect(m_bound);
    return true;
}

bool DanmakuText::update()
{
    if(m_last_clock == 0){
        m_last_clock = clock();
        return true;
    }
    m_last_clock = clock();
    if(m_delTag) return false;
    return true;
}

void DanmakuText::calcBound(QPainter *painter)
{
    if(!m_boundReady){
        painter->setFont(QFont(DANMAKU_DEFAULT_FONT, m_font_size, DANMAKU_DEFAULT_TEXT_WEIGHT));
        m_bound = painter->fontMetrics().boundingRect(m_text);
    }
    m_boundReady = true;
}

clock_t DanmakuText::getFrameInterval()
{
    return m_last_clock? (clock()-m_last_clock)/(CLOCKS_PER_SEC/DANMAKU_FPS) :0;
}
