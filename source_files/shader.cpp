#include "../includes/glad.h"
#include "../includes/shader.h"
#include <iostream>

Shader::Shader(){}
Shader::~Shader(){
    glDeleteProgram(programID);
}

unsigned long Shader::getFileLength(FILE* fp){
    unsigned long fileLength;

    fseek(fp, 0, SEEK_END);
    fileLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return fileLength;
}

char* Shader::loadShaderFromFile(const char* filePath){
    char ch;
    unsigned long fileLength;
    int i = 0;
    FILE *fp;

    fp = fopen(filePath, "r");
    if (fp == NULL){
        fprintf(stderr, "Failed to open the file\n");
    }

    fileLength = getFileLength(fp);

    char* shaderCode =
        new char[sizeof(*shaderCode) * (fileLength + 1)];

    while ((ch = fgetc(fp)) != EOF){
        shaderCode[i++] = ch;
    }
    shaderCode[i] = '\0';

    fclose(fp);
    return shaderCode;
}

void Shader::checkCompileError(GLuint ID, entityType type){
    GLint success;
    GLchar info_log[1024];
    if (type == entityType::PROGRAM){
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success){
            glGetProgramInfoLog(ID, 1024, NULL, info_log);
            std::cerr << "Exiting due to program linking error : " << info_log << std::endl;
            exit(1);
        }
    }
    else {
        const char* shaderName = type == entityType::F_SHADER ? "fragment" : "vertex";
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if (!success) {// if not success, then get info log
            glGetShaderInfoLog(ID, 1024, NULL, info_log);
            std::cerr << "Exiting due to " << shaderName <<" shader compilation error : " << info_log << std::endl;
            exit(1);
        }
    }
}

int Shader::loadVertexShader(const char *filePath){
    const GLchar* shaderPtr = loadShaderFromFile(filePath);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderPtr, NULL);
    glCompileShader(vertexShader);
    checkCompileError(vertexShader, entityType::V_SHADER);

    delete shaderPtr;
    return 0;
}

int Shader::loadFragmentShader(const char *filePath){
    const GLchar* shaderPtr = loadShaderFromFile(filePath);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderPtr, NULL);
    glCompileShader(fragmentShader);
    checkCompileError(fragmentShader, entityType::F_SHADER);

    delete shaderPtr;
    return 0;
}

int Shader::loadProgram(){
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    checkCompileError(programID, entityType::PROGRAM);

    // free up shader occupied memory
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
}

void Shader::useGlProgram(){
    glUseProgram(programID);
}
