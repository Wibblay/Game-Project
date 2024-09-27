#version 330 core
in vec3 fragColor;
out vec4 FragColor;

uniform bool isWireframe;
uniform vec3 outlineColor;

void main()
{
    if (isWireframe)
    {
        FragColor = vec4(outlineColor, 1.0);
    }
    else
    {
        FragColor = vec4(fragColor, 1.0);
    }
}
