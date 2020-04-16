#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include "fbxsdk.h"

#include <iostream>

//-------------------------------------------------------------
//                       fbx sdk
//-------------------------------------------------------------


/* Tab character ("\t") counter */
int numTabs = 0;

/**
 * Print the required number of tabs.
 */
void PrintTabs() {
    for(int i = 0; i < numTabs; i++)
        printf("\t");
}

/**
 * Return a string-based representation based on the attribute type.
 */
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
    switch(type) {
        case FbxNodeAttribute::eUnknown: return "unidentified";
        case FbxNodeAttribute::eNull: return "null";
        case FbxNodeAttribute::eMarker: return "marker";
        case FbxNodeAttribute::eSkeleton: return "skeleton";
        case FbxNodeAttribute::eMesh: return "mesh";
        case FbxNodeAttribute::eNurbs: return "nurbs";
        case FbxNodeAttribute::ePatch: return "patch";
        case FbxNodeAttribute::eCamera: return "camera";
        case FbxNodeAttribute::eCameraStereo: return "stereo";
        case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
        case FbxNodeAttribute::eLight: return "light";
        case FbxNodeAttribute::eOpticalReference: return "optical reference";
        case FbxNodeAttribute::eOpticalMarker: return "marker";
        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
        case FbxNodeAttribute::eBoundary: return "boundary";
        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
        case FbxNodeAttribute::eShape: return "shape";
        case FbxNodeAttribute::eLODGroup: return "lodgroup";
        case FbxNodeAttribute::eSubDiv: return "subdiv";
        default: return "unknown";
    }
}

/**
 * Print an attribute.
 */
void PrintAttribute(FbxNodeAttribute* pAttribute) {
    if(!pAttribute) return;

    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

/**
 * Print a node, its attributes, and all its children recursively.
 */
void PrintNode(FbxNode* pNode) {
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
        nodeName,
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
        );
    numTabs++;

    // Print the node's attributes.
    for(int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // Recursively print the children.
    for(int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}


//-------------------------------------------------------------
//          https://codelabo.com/posts/20200228164918
//-------------------------------------------------------------


void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);
void DisplayMesh(FbxNode* pNode);
void DisplayIndex(FbxMesh* mesh);
void DisplayPosition(FbxMesh* mesh);

void DisplayContent(FbxNode* node)
{
    FbxNodeAttribute::EType lAttributeType;

    if (node->GetNodeAttribute() == NULL){
        std::cout << "NULL Node Attribute\n\n";
    }
    else{
        lAttributeType = (node->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
        default:
            break;

        case FbxNodeAttribute::eMesh:
            DisplayMesh(node);
            break;
        }
    }


    for (int i = 0; i < node->GetChildCount(); i++){
        DisplayContent(node->GetChild(i));
    }
}

void DisplayContent(FbxScene* scene)
{
    FbxNode* node = scene->GetRootNode();

    if (node){
        for (int i = 0; i < node->GetChildCount(); i++){
            DisplayContent(node->GetChild(i));
        }
    }
}

void DisplayMesh(FbxNode* node)
{
    FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();

    std::cout << "\n\nMesh Name: " << (char *)node->GetName() << std::endl;

    DisplayIndex(mesh);
    DisplayPosition(mesh);

}

void DisplayIndex(FbxMesh* mesh)
{
    //総ポリゴン数
    int polygonNum = mesh->GetPolygonCount();

    //p個目のポリゴンへの処理
    for (int p = 0; p < polygonNum; ++p){
        //p個目のポリゴンのn個目の頂点への処理
        for (int n = 0; n < 3; ++n){
            int index = mesh->GetPolygonVertex(p, n);
            std::cout << "index[" << p+n <<"] : " << index << std::endl;
        }
    }
}

void DisplayPosition(FbxMesh* mesh){
    int positionNum = mesh->GetControlPointsCount();	// 頂点数
    FbxVector4* position = mesh->GetControlPoints();	// 頂点座標配列

    for (int i = 0; i < positionNum; ++i){
        std::cout << "position[" << i << "] : ("
            << position[i][0] << ","
            << position[i][1] << ","
            << position[i][2] << ","
            << position[i][3] << ")" << std::endl;
    }
}


//-------------------------------------------------------------
//                           main
//-------------------------------------------------------------

int main(int argc, char** argv)
{

    // file name
    const char* lFilename = "E:/3D Objects/Cube.fbx";

    // Initialize the SDK manager
    // メモリ管理を行う
    FbxManager* lSdkManager = FbxManager::Create();

    // Create the IO settings
    FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    // managerにio設定を渡す
    lSdkManager->SetIOSettings(ios);

    // Create an importer using the SDK manager.
    // managerを使ってimporterを初期化する
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");

    // Use the first argument as the filename for the importer.
    // importerをfilenameとio settingで初期化する
    if(!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }else{
        qDebug() << "Initialize succeed";
    }

    // Create a new scene so that it can be populated by the imported file.
    // sceneを作成する
    FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene");

    // Import the contents of the file into the scene.
    // sceneにfileをimport
    lImporter->Import(lScene);

    // The file is imported; so get rid of the importer.
    // importが終了したらimporterは必要ない
    lImporter->Destroy();

    DisplayContent(lScene);

    // Print the nodes of the scene and their attributes recursively.
    // Note that we are not printing the root node because it should
    // not contain any attributes.
    FbxNode* lRootNode = lScene->GetRootNode();
    if(lRootNode) {
        for(int i = 0; i < lRootNode->GetChildCount(); i++)
            PrintNode(lRootNode->GetChild(i));
    }
    // Destroy the SDK manager and all the other objects it was handling.
    lSdkManager->Destroy();

    return 0;
}

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}
