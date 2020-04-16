in vec4 position;
uniform mat4 mvpMatrix;

void main(void)
{
    gl_Position = mvpMatrix * position;
}
