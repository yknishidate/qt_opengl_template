#include "glwidget.h"
#include <QFileInfo>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

void GLWidget::initializeGL()
{
    // QOpenGLFunctionsを現在のコンテキストに関連付けする
    initializeOpenGLFunctions();
    // version確認
    qDebug() << "GL version  :" << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    qDebug() << "GLSL version:" << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

    // 背景を設定
    glClearColor(0.7f, 0.7f, 0.4f, 1.0f);

    // 頂点を用意
    int numVertices = 3;
    float vertices[] {
         0.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, 0.0f, 0.0f, 1.0f,
    };

    // -----vertex bufferをセットアップする-----
    // bufferを作成し、さらに現在のコンテキストにバインド
//    vertexBuffer.create();
    // bufferの用途を設定する
//    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    // bufferの領域確保を行い、さらにverticesデータを入れる
//    const int numDimensions = 4;
//    int bufferSize = numVertices * numDimensions * sizeof(float);
//    vertexBuffer.allocate(vertices, bufferSize);


    // shader programをセットアップする
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex_shader.vsh");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.fsh");
    qDebug() << "link: " << shaderProgram.link();
    qDebug() << "bind: " << shaderProgram.bind();

    // locationを取得
//    variables_location["position"] = 0;
    variables_location["vpMatrix"] = shaderProgram.uniformLocation("vpMatrix");

}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // view matrix
    QVector3D eyePos(0, 0, 5);
    QVector3D targetPos(0, 0, 5);
    QVector3D upVector(0, 1, 0);
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(eyePos, targetPos, upVector);

    // vp matrixをshaderに送る
    QMatrix4x4 vpMatrix = projectionMatrix * viewMatrix;
    shaderProgram.setUniformValue(variables_location["vpMatrix"], vpMatrix);

    int numDimensions = 2;
    static float vertices[] {
         0.0f, 1.0f,
        -1.0f, 0.0f,
         1.0f, 0.0f,
    };

    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeArray(0, vertices, numDimensions);

    glDrawArrays(GL_TRIANGLES, /*firstVertex=*/ 0, /*numVertex=*/ 3);
}

void GLWidget::resizeGL(int w, int h)
{
    // viewportの領域を設定
//    glViewport( 0, 0, w, h);

    // projection matrixを単位行列にリセット
    projectionMatrix.setToIdentity();

    // projection matrixのperspectiveを設定
    float verticalAngle = 60.0;
    float aspectRatio = float(w)/h;
    float nearPlane = 0.01;
    float farPlane = 1000.0;
    projectionMatrix.perspective(verticalAngle, aspectRatio, nearPlane, farPlane);
}
