#version 330 core

uniform vec3 cubeColor;

out vec4 FragColor;

void main(){
    FragColor = vec4(cubeColor, 1.0); // all bright light source
}
