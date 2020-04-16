#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QtDebug>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "mesh.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT // QObjectの派生クラスを作成する際に必要なマクロ

public:
    GLWidget(QWidget *parent);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer;
    QOpenGLShaderProgram shaderProgram;
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 viewMatrix;
    Mesh mesh;
};
#endif // GLWIDGET_H
