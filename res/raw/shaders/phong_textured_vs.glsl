#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 uv; // vec3 because of assimp

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 frag_pos;
out vec3 frag_normal;
out vec3 frag_uv;

void main()
{
    frag_pos = vec3(model * vec4(position, 1.0));
    frag_normal = mat3(transpose(inverse(model))) * normal;
    frag_uv = uv;

    gl_Position = proj * view * vec4(frag_pos, 1.0);
}
