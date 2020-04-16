#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class Mesh : protected QOpenGLFunctions
{
public:
    Mesh();
    void init();
    void setPositions(const QVector<QVector3D>&);
    void setIndices(const QVector<GLuint>&);
    void draw(QOpenGLShaderProgram&);

private:
    QVector<QVector3D> positions;
    QVector<GLuint> indices;

    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
};

#endif // MESH_H
