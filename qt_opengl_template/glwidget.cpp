#include "glwidget.h"
#include "fbx_importer.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      vertexBuffer(QOpenGLBuffer::VertexBuffer),
      indexBuffer(QOpenGLBuffer::IndexBuffer)
{

}

void GLWidget::initializeGL()
{
    // QOpenGLFunctionsを現在のコンテキストに関連付けする
    initializeOpenGLFunctions();

    // 背景を設定
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

    // ---------- vbo ----------
    // bufferを作成する
    vertexBuffer.create();
    // bufferを現在のコンテキストにバインド
    vertexBuffer.bind();
    // bufferの用途を設定する
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    // bufferの領域確保を行い、さらにverticesデータを入れる
//    int vertexBufferSize = numVertices * 4 * sizeof(GLfloat); // 4 = numDimensions
//    vertexBuffer.allocate(vertices, vertexBufferSize);

    // ---------- ibo ----------
    // 処理は同上
    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    int indexBufferSize = numIndices * sizeof(GLuint);
//    indexBuffer.allocate(indices, indexBufferSize);


    // shader programをセットアップする
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex_shader.vsh");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.fsh");
    shaderProgram.link();
    shaderProgram.bind();

    importFbx("E:/3D Objects/Cube.fbx", vertexBuffer, indexBuffer);
}

void GLWidget::paintGL()
{
    static int frame = 0;

    // view matrixを計算する
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(/*eyePos*/ QVector3D(1, 1, 1), /*targetPos*/ QVector3D(0, 0, 0),
                      /*upVector*/ QVector3D(0, 1, 0));

    // model matrixを作成し、回転させる
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.rotate(frame, QVector3D(0, 1, 0));

    // mvp matrixを計算し、shaderに送る
    QMatrix4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // vertex bufferをshaderに送る
    shaderProgram.enableAttributeArray("position");
    shaderProgram.setAttributeBuffer("position", GL_FLOAT, /*offset*/ 0, /*tupleSize*/ 3);

    // indexを利用して描画する
    // indicesには、iboを使わない場合は配列データのポインタを渡す
    // iboを使う場合はiboの位置をbyteで指定する
    glDrawElements(GL_TRIANGLES, /*numIndices*/ 36, /*type*/ GL_UNSIGNED_INT, /*indices*/ 0);

    frame++;
    update();
}

void GLWidget::resizeGL(int w, int h)
{
    // projection matrixを単位行列にする
    projectionMatrix.setToIdentity();
    // projection matrixのperspectiveを設定
    projectionMatrix.perspective(/*verticalAngle*/ 60.0, /*aspectRatio*/ float(w)/h,
                                 /*nearPlane*/ 0.01, /*farPlane*/ 1000.0);
}
