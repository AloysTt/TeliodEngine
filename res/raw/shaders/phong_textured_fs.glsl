#version 330 core

in vec3 frag_pos;
in vec3 frag_normal;
in vec3 frag_uv; // vec3 because of assimp

out vec4 fragColor;

uniform sampler2D texture1;

uniform vec3 lightDir;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform vec3 matSpecular;
uniform float matShininess;

uniform vec3 viewPos;

void main()
{
    vec3 viewDir = normalize(viewPos - frag_pos);
    // diffuse shading
    float diff = max(dot(frag_normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, frag_normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matShininess);
    // combine results
    vec3 ambient = lightAmbient * vec3(texture(texture1, frag_uv.xy));
    vec3 diffuse = lightDiffuse * diff * vec3(texture(texture1, frag_uv.xy));
    vec3 specular = lightSpecular * spec * matSpecular;

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}
