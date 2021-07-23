#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <iostream>

class Shader {
private:
  unsigned int programID;
  GLuint vertexShader;
  GLuint fragmentShader;
  enum class entityType { V_SHADER, F_SHADER, PROGRAM };

  char* loadShaderFromFile(const char *filePath);
  void checkCompileError(GLuint s, entityType type);
  unsigned long getFileLength(FILE *fp);

public:
  Shader();
  ~Shader();
  int loadVertexShader(const char *filePath);
  int loadFragmentShader(const char *filePath);
  int loadProgram();
  void useGlProgram();
};

#endif // SHADER_H_
