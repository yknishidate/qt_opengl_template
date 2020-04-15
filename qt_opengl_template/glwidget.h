#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT // QObjectの派生クラスを作成する際に必要なマクロ

public:
    GLWidget(QWidget *parent);

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    QOpenGLShaderProgram shaderProgram;

};
#endif // GLWIDGET_H
