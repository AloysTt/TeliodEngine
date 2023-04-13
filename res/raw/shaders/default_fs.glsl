#version 330 core

in vec3 frag_normal;

out vec4 fragColor;

void main()
{
    fragColor = vec4(clamp(frag_normal, 0.0, 1.0), 1.0);
}
