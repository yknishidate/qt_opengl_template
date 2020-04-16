#include "mesh.h"

Mesh::Mesh():
    vbo(QOpenGLBuffer::VertexBuffer),
    ibo(QOpenGLBuffer::IndexBuffer)
{

}

void Mesh::setPositions(QVector<QVector3D> positions)
{
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->positions = positions;
    int bufferSize = positions.size() * 3 * sizeof(GLfloat); // 3 = 3D
    vbo.allocate(&this->positions[0], bufferSize);
}

void Mesh::setIndices(QVector<GLuint> indices)
{
    ibo.create();
    ibo.bind();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int bufferSize = indices.size() * sizeof(GLuint);
    ibo.allocate(&this->indices[0], bufferSize);
}

void Mesh::render(QOpenGLShaderProgram shaderProgram)
{

}
