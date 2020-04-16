#include "mesh.h"

Mesh::Mesh():
    vbo(QOpenGLBuffer::VertexBuffer),
    ibo(QOpenGLBuffer::IndexBuffer)
{
    // glDrawを使えるようにする
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

void Mesh::setVertices(const QVector<Vertex>& vertices)
{
    this->vertices = vertices;

    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    int bufferSize = vertices.size() * sizeof(Vertex);
    vbo.allocate(&vertices[0], bufferSize);
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

    // ---------- position only ----------
    // vboをshaderに送る
//    shaderProgram.enableAttributeArray("position");
//    shaderProgram.setAttributeBuffer("position", GL_FLOAT, /*offset*/ 0, /*tupleSize*/ 3); // 3 = 3D


    // ---------- all attribute ----------
    // これらの関数は現在バインドされているvboに対して行われる
    shaderProgram.enableAttributeArray("position");
    shaderProgram.setAttributeBuffer("position", GL_FLOAT, /*offset*/ offsetof(Vertex, position),
                                     /*tupleSize*/ 3, /*stride*/ sizeof(Vertex)); // 3 = 3D
    shaderProgram.enableAttributeArray("normal");
    shaderProgram.setAttributeBuffer("normal", GL_FLOAT, /*offset*/ offsetof(Vertex, normal),
                                     /*tupleSize*/ 3, /*stride*/ sizeof(Vertex)); // 3 = 3D
//    shaderProgram.enableAttributeArray("color");
//    shaderProgram.setAttributeBuffer("color", GL_FLOAT, /*offset*/ sizeof(Vertex), /*tupleSize*/ 3); // 3 = 3D
//    shaderProgram.enableAttributeArray("texcoord");
//    shaderProgram.setAttributeBuffer("texcoord", GL_FLOAT, /*offset*/ sizeof(Vertex), /*tupleSize*/ 2); // 2 = 2D


    // indexを利用して描画する
    // indicesには、iboを使わない場合は配列データのポインタを渡す
    // iboを使う場合はiboの位置をbyteで指定する
    glDrawElements(GL_TRIANGLES, indices.size(), /*type*/ GL_UNSIGNED_INT, /*indices*/ 0);

}
