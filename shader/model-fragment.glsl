#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct LightSource {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    //vec3 specular;
};
uniform LightSource light;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(){
    //vec4 specular = texture(texture_specular1, TexCoords);
    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;

    vec3 result = (ambient + diffuse);
    FragColor = vec4(result, 1.0);
}
