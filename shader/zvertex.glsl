#version 330 core

layout (location = 0) in vec3 aVertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main(){
    vec3 FragPos = vec3(model * vec4(aVertex, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
    // gl_Position = projection * view * model * vec4(aVertex,1.0f);
}
