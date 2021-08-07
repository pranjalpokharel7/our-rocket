#version 330 core

layout (location = 0) in vec3 aVertex;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 inv_model; // if I make mat3 data structure for this one? CPU calculation?
uniform mat4 view;
uniform mat4 projection;

void main(){
    FragPos = vec3(model * vec4(aVertex, 1.0));
    Normal = mat3(inv_model) * aNormal;
    TexCoords = aTexCoord;
    gl_Position = projection * view * model * vec4(aVertex, 1.0);
}
