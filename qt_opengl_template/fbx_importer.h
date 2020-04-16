#ifndef FBX_IMPORTER_H
#define FBX_IMPORTER_H

#include <QtDebug>
#include <iostream>
#include <QOpenGLBuffer>
#include <QVector3D>
#include "fbxsdk.h"


int importFbx(const char* lFilename, QOpenGLBuffer& vertexBuffer, QOpenGLBuffer& indexBuffer)
{

    // メモリ管理を行うmanagerを初期化
    FbxManager* manager = FbxManager::Create();

    // IO settingsを作成し、managerに渡す
    FbxIOSettings *ios = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ios);

    // managerを使ってimporterを初期化する
    FbxImporter* lImporter = FbxImporter::Create(manager,"");

    // importerをfilenameとio settingで初期化する
    if(!lImporter->Initialize(lFilename, -1, manager->GetIOSettings())) {
        qDebug() << "Call to FbxImporter::Initialize() failed.";
        qDebug() << "Error returned: %s\n\n" << lImporter->GetStatus().GetErrorString();
        exit(-1);
    }

    // sceneを作成する
    FbxScene* scene = FbxScene::Create(manager,"");

    // sceneにfileをimport
    lImporter->Import(scene);

    // importが終了したらimporterは必要ないので消す
    lImporter->Destroy();

    // polygonを三角形にする
    FbxGeometryConverter geometryConverter(manager);
    geometryConverter.Triangulate(scene, true);

    // オブジェクトを取得する
    FbxNode* node = scene->GetRootNode()->GetChild(0);
    if (node->GetNodeAttribute() == NULL){
        qDebug() << "node attr is NULL";
        exit(-1);
    }

    // オブジェクトからメッシュを取得する
    auto lAttributeType = (node->GetNodeAttribute()->GetAttributeType());
    if(lAttributeType != FbxNodeAttribute::eMesh){
        qDebug() << "attr type is not eMesh";
        exit(-1);
    }
    FbxMesh* mesh = static_cast<FbxMesh*>(node->GetNodeAttribute());

    // positionを取得する
    int numPosition = mesh->GetControlPointsCount();	// 頂点数
    FbxVector4* position = mesh->GetControlPoints();	// 頂点座標配列
    QVector<QVector3D> vertices;
    for (int i = 0; i < numPosition; ++i){
        QVector3D vertex(position[i][0]/200.0, position[i][1]/200.0, position[i][2]/200.0);
        vertices.push_back(vertex);
    }

    // indexを取得する
    int numPolygon = mesh->GetPolygonCount();
    QVector<GLuint> indices;
    for (int p = 0; p < numPolygon; ++p){
        //p個目のポリゴンのn個目の頂点への処理
        for (int n = 0; n < 3; ++n){
            GLuint index = mesh->GetPolygonVertex(p, n);
            printf("%d, ", index);
            indices.push_back(index);
        }
    }

    // vbo allocate
    // QVectorの &v は &v[0] と異なるため注意
    int vertexBufferSize = numPosition * 3 * sizeof(GLfloat); // 3 = 3D
    vertexBuffer.allocate(&vertices[0], vertexBufferSize);

    // ibo allocate
    int indexBufferSize = numPolygon * 3 * sizeof(GLuint); // 3 = triangle
    indexBuffer.allocate(&indices[0], indexBufferSize);

    // managerと管理しているオブジェクトを全て破棄する
    manager->Destroy();

    return indices.size();
}



#endif // FBX_IMPORTER_H
