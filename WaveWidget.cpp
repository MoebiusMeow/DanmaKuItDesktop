#include "WaveWidget.h"
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

    waveProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Assets/Shaders/plainUV.vert");
    waveProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Assets/Shaders/waveShader.frag");
    waveProgram.link();
    waveProgram.bind();
    waveProgram.setUniformValue("iResolution", QPointF(width(), height()));
    waveProgram.setUniformValue("iTime", (GLfloat)0.0f);
    waveProgram.release();

    resizeGL(width(), height());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WaveWidget::animate);
    timer->start(16);
}

void WaveWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    VAO->bind();
    waveProgram.bind();
    waveProgram.setUniformValue("iResolution", QVector2D(width(), height()));
    waveProgram.setUniformValue("iTime", (GLfloat)(QTime::currentTime().msecsSinceStartOfDay() * 0.001f));
    waveProgram.setUniformValue("iRoundRadius", (GLfloat)borderRadius);
    //qDebug() << QVector2D(100, 100);
    f->glViewport(0, 0, width(), height());
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    f->glClear(GL_COLOR_BUFFER_BIT);
    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    waveProgram.release();
    VAO->release();
}

void WaveWidget::resizeGL(int w, int h)
{
    makeCurrent();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glViewport(0, 0, w, h);
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

WaveWidget::~WaveWidget()
{
    makeCurrent();
    VBO->destroy();
}
