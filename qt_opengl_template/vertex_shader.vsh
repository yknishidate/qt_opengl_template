#version 400
in vec3 position;
in vec3 normal;
uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;

out vec3 w_normal;

void main(void)
{
    gl_Position = mvpMatrix * vec4(position, 1.0);
    w_normal = normalMatrix * normal;
}
