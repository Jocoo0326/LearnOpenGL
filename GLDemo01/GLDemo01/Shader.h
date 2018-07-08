#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include "utils.h"

class Shader
{
public:
  // shader id
  GLuint ID;

  // constructor reads shader source file and builds shader
  Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

  // use/activate the shader
  void use();

  // utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMatrix(const std::string &name, glm::mat4 matrix);

  ~Shader();
};

#endif // !SHADER_H
