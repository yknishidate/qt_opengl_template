#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

struct Vertex{
    QVector3D position;
    QVector3D normal;
    QVector3D color;
    QVector2D texcoord;
};

class Mesh : protected QOpenGLFunctions
{
public:
    Mesh();
    void init();
    void setPositions(const QVector<QVector3D>&);
    void setVertices(const QVector<Vertex>&);
    void setIndices(const QVector<GLuint>&);
    void draw(QOpenGLShaderProgram&);

private:
    QVector<QVector3D> positions;
    QVector<Vertex> vertices;
    QVector<GLuint> indices;

    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
};

#endif // MESH_H
