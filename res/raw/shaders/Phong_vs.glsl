#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 uv0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

out vec3 FragPos;
out vec3 FragNormal;
out vec2 FragUV0;
out vec3 FragLightPos;


void main() 
{
  FragPos = vec3( model * vec4( position, 1. ) );
  gl_Position = projection * view * vec4( FragPos, 1. );

  mat3 normalMatrix = mat3(transpose(inverse(model)));
  FragUV0 = uv0.xy;
  FragNormal = normalMatrix * normal;
  FragLightPos = vec3( view * vec4( lightPos, 1. ) );

  /*
  mat3 normalMatrix = mat3(transpose(inverse(model)));
  o_uv0 = uv0;
  vec4 positionWorld = model * vec4(position, 1.0);
  o_positionWorld = positionWorld.xyz;
  o_normalWorld = normalMatrix * normal;
  gl_Position = projection * view * positionWorld;
  */
}