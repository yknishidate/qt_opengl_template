#version 330
//attribute highp vec4 qt_Vertex;
//attribute highp vec4 qt_MultiTexCoord0;
//uniform highp mat4 qt_ModelViewProjectionMatrix;
//varying highp vec4 qt_TexCoord0;
//layout(location = 0) in vec4 position;
in vec4 position;
uniform mat4 vpMatrix;

void main(void)
{
//    gl_Position = vpMatrix * position;
    gl_Position = position;
//    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
//    qt_TexCoord0 = qt_MultiTexCoord0;
}
