#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
//in vec3 Normal;
//out vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(){
    vec4 diffuse = texture(texture_diffuse1, TexCoords);
    //vec4 specular = texture(texture_specular1, TexCoords);
    //vec4 color = vec4(0.9, 0.9, 0.9, 1.0);
    FragColor = diffuse;
    //FragColor = color;
}
