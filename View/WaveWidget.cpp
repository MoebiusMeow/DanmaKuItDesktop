#include "WaveWidget.h"
#include <QtWidgets>
#include <QOpenGLFunctions>
#include <QTime>
#include <QTimer>
#include <QVector2D>

GLfloat WaveWidget::vertices[18] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,

     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f
};

WaveWidget::WaveWidget(QWidget *parent) : QOpenGLWidget(parent), borderRadius(0.0f)
{
    VAO = new QOpenGLVertexArrayObject(this);
    VBO = new QOpenGLBuffer();
    setAttribute(Qt::WA_TranslucentBackground);
    themeTrans = 0.0f;
}

void WaveWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    VAO->create();
    VAO->bind();
    VBO->create();
    VBO->bind();
    VBO->allocate(vertices, sizeof vertices);
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    VAO->release();

    qreal ratio = screen()->devicePixelRatio();
    waveProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Assets/Shaders/plainUV.vert");
    waveProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Assets/Shaders/waveShader.frag");
    waveProgram.link();
    waveProgram.bind();
    waveProgram.setUniformValue("iResolution", ratio * QPointF(width(), height()));
    waveProgram.setUniformValue("iTime", (GLfloat)0.0f);
    waveProgram.release();

    resizeGL(width(), height());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WaveWidget::animate);
    timer->start(16);
}

void WaveWidget::paintGL()
{
    qreal ratio = screen()->devicePixelRatio();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    VAO->bind();
    waveProgram.bind();
    waveProgram.setUniformValue("iResolution", ratio * QVector2D(width(), height()));
    waveProgram.setUniformValue("iTime", (GLfloat)(QTime::currentTime().msecsSinceStartOfDay() * 0.001f));
    waveProgram.setUniformValue("iThemeTrans", themeTrans);
    waveProgram.setUniformValue("iRoundRadius", (GLfloat)ratio * borderRadius);
    //qDebug() << QVector2D(100, 100);=
    f->glViewport(0, 0, (int)(width() * ratio), (int)(height() * ratio));
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //f->glClear(GL_COLOR_BUFFER_BIT);
    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    waveProgram.release();
    VAO->release();
}

void WaveWidget::resizeGL(int w, int h)
{
    makeCurrent();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    qreal ratio = screen()->devicePixelRatio();
    f->glViewport(0, 0, (int)(w * ratio), (int)(h * ratio));
    //qDebug() << ratio;
}

void WaveWidget::setBorderRadius(float radius)
{
    borderRadius = targetBorderRadius = radius;
}

void WaveWidget::setBorderRadiusLerp(float from, float to)
{
    borderRadius = from;
    targetBorderRadius = to;
}

void WaveWidget::animate()
{
    this->update();
    borderRadius = 0.85 * borderRadius + 0.15 * targetBorderRadius;
}

float WaveWidget::getThemeTrans()
{
    return themeTrans;
}

void WaveWidget::setThemeTrans(float value)
{
    themeTrans = value;
}

void WaveWidget::animateTheme(float endValue, int duration)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "themeTrans", this);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setEndValue(endValue);
    animation->setDuration(duration);
    animation->start();
}


WaveWidget::~WaveWidget()
{
    makeCurrent();
    VBO->destroy();
}
