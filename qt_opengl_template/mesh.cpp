#include "mesh.h"

Mesh::Mesh():
    vbo(QOpenGLBuffer::VertexBuffer),
    ibo(QOpenGLBuffer::IndexBuffer)
{
}

void Mesh::init()
{
    // meshを作ったらすぐに実行する
    // QOpenGLFunctionsを継承しているのはこの関数のため
    initializeOpenGLFunctions();
}

void Mesh::setPositions(const QVector<QVector3D>& positions)
{
    this->positions = positions;

    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int bufferSize = positions.size() * 3 * sizeof(GLfloat); // 3 = 3D
    vbo.allocate(&positions[0], bufferSize);
}

void Mesh::setIndices(const QVector<GLuint>& indices)
{
    this->indices = indices;

    ibo.create();
    ibo.bind();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int bufferSize = indices.size() * sizeof(GLuint);
    ibo.allocate(&indices[0], bufferSize);
}

void Mesh::draw(QOpenGLShaderProgram& shaderProgram)
{
    vbo.bind();
    ibo.bind();

    // vboをshaderに送る
    shaderProgram.enableAttributeArray("position");
    shaderProgram.setAttributeBuffer("position", GL_FLOAT, /*offset*/ 0, /*tupleSize*/ 3); // 3 = 3D

    // indexを利用して描画する
    // indicesには、iboを使わない場合は配列データのポインタを渡す
    // iboを使う場合はiboの位置をbyteで指定する
    glDrawElements(GL_TRIANGLES, indices.size(), /*type*/ GL_UNSIGNED_INT, /*indices*/ 0);

}
