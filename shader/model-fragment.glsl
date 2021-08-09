#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct LightSource {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constantAtten;
    float linearAtten;
    float quadraticAtten;
};
uniform LightSource light;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(){
    vec3 textureDiffuse = texture(texture_diffuse1, TexCoords).rgb;
    vec3 textureSpecular = texture(texture_diffuse1, TexCoords).rgb;

    // ambient
    vec3 ambient = light.ambient * textureDiffuse;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // hard-coded shininess for now
    vec3 specular = light.specular * (spec * textureSpecular); // our material does not have a specular tecture yet

    // attenuation
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constantAtten + light.linearAtten * dist + light.quadraticAtten * dist * dist);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // output
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
