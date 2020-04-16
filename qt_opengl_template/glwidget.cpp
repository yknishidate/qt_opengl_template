#include "glwidget.h"
#include "fbx_importer.h"

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

    // ---------- diamond mesh ----------
//    QVector<QVector3D> positions;
//    positions << QVector3D( 0.0f,  0.5f, 0.0f)
//              << QVector3D(-0.5f,  0.0f, 0.0f)
//              << QVector3D( 0.0f, -0.5f, 0.0f)
//              << QVector3D( 0.5f,  0.0f, 0.0f);
//    QVector<GLuint> indices = { 0, 1, 3, 1, 2, 3 };

//    mesh = QSharedPointer<Mesh>(new Mesh);
//    mesh->setPositions(positions);
//    mesh->setIndices(indices);

    // import from fbx
    mesh = importFbx("E:/3D Objects/teapot.fbx");

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
    viewMatrix.lookAt(/*eyePos*/ QVector3D(1, 1, 1), /*targetPos*/ QVector3D(0, 0, 0),
                      /*upVector*/ QVector3D(0, 1, 0));

    // model matrixを作成し、回転させる
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.rotate(frame, QVector3D(0, 1, 0));

    // mvp matrixを計算し、shaderに送る
    QMatrix4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // 描画する
    mesh->draw(shaderProgram);

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
