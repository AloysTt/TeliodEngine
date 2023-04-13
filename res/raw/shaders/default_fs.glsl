#version 330 core

in vec3 frag_normal;

out vec4 fragColor;

void main()
{
    fragColor = vec4(frag_normal, 1.0);
}
