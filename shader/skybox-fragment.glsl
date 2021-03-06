#version 330 core

out vec4 FragColor;

in vec3 TexCoords;
uniform samplerCube skybox;

void main(){
    //FragColor = vec4(1.0, 0.1, 0.1, 1.0); // all bright light source
    FragColor = texture(skybox, TexCoords);
}
