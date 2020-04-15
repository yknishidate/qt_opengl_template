#version 330
#extension GL_NV_shadow_samplers_cube : enable
//uniform sampler2D qt_Texture0;
//varying highp vec4 qt_TexCoord0;
//out vec4 fColor;

void main(void)
{
//    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
//    fColor = vec4(1.0, 1.0, 1.0, 1.0);
}
