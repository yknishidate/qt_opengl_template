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

    // 頂点を用意
    int numVertices = 3;
    float vertices[] {
         0.0f,  0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f,
    };

    // bufferを作成する
    vertexBuffer.create();
    // bufferを現在のコンテキストにバインド
    vertexBuffer.bind();
    // bufferの用途を設定する
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    // bufferの領域確保を行い、さらにverticesデータを入れる
    int bufferSize = numVertices * 4 * sizeof(float); // 4 = numDimensions
    vertexBuffer.allocate(vertices, bufferSize);

    // shader programをセットアップする
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex_shader.vsh");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.fsh");
    shaderProgram.link();
    shaderProgram.bind();
}

void GLWidget::paintGL()
{
    static int frame = 0;

    // view matrixを計算する
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(/*eyePos   = */ QVector3D(1, 1, 1), /*targetPos = */ QVector3D(0, 0, 0),
                      /*upVector = */ QVector3D(0, 1, 0));

    // model matrixを作成し、回転させる
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.rotate(frame, QVector3D(0, 1, 0));

    // mvp matrixを計算し、shaderに送る
    QMatrix4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // vertex bufferをshaderに送る
    shaderProgram.enableAttributeArray("position");
    shaderProgram.setAttributeBuffer("position", GL_FLOAT, /*offset = */ 0, /*tupleSize = */ 4);

    glDrawArrays(GL_TRIANGLES, /*firstVertex = */ 0, /*numVertex = */ 3);

    frame++;
    update();
}

void GLWidget::resizeGL(int w, int h)
{
    // projection matrixを単位行列にリセット
    projectionMatrix.setToIdentity();

    // projection matrixのperspectiveを設定
    projectionMatrix.perspective(/*verticalAngle = */ 60.0, /*aspectRatio = */ float(w)/h,
                                 /*nearPlane     = */ 0.01, /*farPlane    = */ 1000.0);
}
