#version 330 core

in vec3 vertexNormal;
out vec4 FragColor;

void main()
{
    vec3 rocketColor = vec3(0.8, 0.2, 0.2);
    FragColor = vec4(rocketColor, 1.0);
}
