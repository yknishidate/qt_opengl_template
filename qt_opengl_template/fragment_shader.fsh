#version 400
in vec3 w_normal;
out vec4 fragColor;

void main(void)
{
    vec3 diffuse = vec3(1.0);
    vec3 lightPosition = vec3(0, 10, 10);

    vec3 N = normalize(w_normal);
    vec3 L = normalize(lightPosition);
    float brightness_front = dot(N, L);
    float brightness_back = dot(-N, L);
    float brightness = max(brightness_front, brightness_back);

    fragColor = vec4(diffuse * brightness, 1.0f);

}
