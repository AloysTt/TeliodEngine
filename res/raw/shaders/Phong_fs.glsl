#version 330 core

in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragUV0;
in vec3 FragLightPos;
out vec4 FragColor;

uniform vec4 color;
uniform vec3 lightColor;
uniform sampler2D colorTexture;

struct Material
{
    vec3 ambient; 
    vec3 diffuse; 
    vec3 specular; 
    float shininess;
};
uniform Material material;

void main() 
{
    // Ambient 
    vec3 ambient = lightColor * material.ambient;

    // Diffuse
    vec3 norm = normalize( FragNormal );
    vec3 lightDir = normalize( FragLightPos - FragPos );
    float diff = max( dot( norm, lightDir), 0. );
    vec3 diffuse = lightColor * ( diff * material.diffuse );

    // Specular
    vec3 viewDir = normalize( -FragPos );
    vec3 reflectDir = reflect( -lightDir, norm );
    float spec = pow( max( dot( viewDir, reflectDir ), 0. ), material.shininess );
    vec3 specular = lightColor * ( spec * material.specular );

    vec3 result = ambient + diffuse + specular;
    FragColor = color*vec4( result, 1. );

    //FragColor = color;
    // FragColor = texture(colorTexture, o_uv0) * color;
    // DEBUG: position
    //FragColor = vec4(o_positionWorld, 1.0);
    // DEBUG: normal
    // FragColor = vec4(0.5 * o_normalWorld + vec3(0.5) , 1.0);
    // DEBUG: uv0
    // FragColor = vec4(o_uv0, 1.0);
}
