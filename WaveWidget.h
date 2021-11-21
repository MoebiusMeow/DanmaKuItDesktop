#ifndef WAVEWIDGET_H
#define WAVEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QObject>
#include <QWidget>

class WaveWidget : public QOpenGLWidget
{
    Q_OBJECT
    Q_PROPERTY(float themeTrans READ getThemeTrans WRITE setThemeTrans)

private:
    QOpenGLBuffer *VBO;
    QOpenGLVertexArrayObject *VAO;
    GLuint FBO[2], RBO[2], blurFBO[2];
    GLuint frameTex[2], blurTex[2];
    QOpenGLShaderProgram waveProgram;
    float borderRadius;
    float targetBorderRadius;
    float themeTrans;

    static GLfloat vertices[18];

protected:
    QTimer *timer;

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

public slots:
    void animate();
    float getThemeTrans();
    void setThemeTrans(float value);
    void animateTheme(float endValue, int duration = 1000);

public:
    void setBorderRadius(float radius);
    void setBorderRadiusLerp(float from, float to);

    WaveWidget(QWidget *parent = nullptr);
    ~WaveWidget();
};

#endif // WAVEWIDGET_H
