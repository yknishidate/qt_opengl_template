
#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

void GLWidget::initializeGL()
{
    // QOpenGLFunctionsを現在のコンテキストに関連付けする
    initializeOpenGLFunctions();

    // 背景を設定
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

    // shader programをセットアップする
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex_shader.vsh");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.fsh");
    shaderProgram.link();
    shaderProgram.bind();
}

void GLWidget::paintGL()
{
    // 頂点を用意
    static float vertices[] {
         0.0f,  0.5f,
        -0.5f, -0.5f,
         0.5f, -0.5f,
    };

    shaderProgram.enableAttributeArray("position");
    shaderProgram.setAttributeArray("position", vertices, /*numDimensions = */ 2);

    glDrawArrays(GL_TRIANGLES, /*firstVertex = */ 0, /*numVertex = */ 3);
}
