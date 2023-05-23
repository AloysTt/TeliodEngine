#version 330 core

in vec3 frag_normal;
in vec3 frag_uv; // vec3 because of assimp

out vec4 fragColor;

uniform sampler2D texture1;

void main()
{
//    fragColor = vec4(clamp(frag_normal, 0.0, 1.0), 1.0);
    fragColor = texture(texture1, frag_uv.xy);
}
