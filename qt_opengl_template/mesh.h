#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>


class Mesh
{
public:
    Mesh();
    void setPositions(QVector<QVector3D>);
    void setIndices(QVector<GLuint>);
    void render(QOpenGLShaderProgram);

private:
    QVector<QVector3D> positions;
    QVector<GLuint> indices;

    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
};

#endif // MESH_H
