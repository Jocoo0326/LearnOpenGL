#include "Shader.h"


std::pair<std::string, std::string> readShaderCodeFromFile(const char * vertexShaderPath, const char * fragmentShaderPath)
{
  std::string vertexSource, fragmentSource;
  std::ifstream vShaderFile, fShaderFile;
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try
  {
    // open files
    vShaderFile.open(vertexShaderPath);
    fShaderFile.open(fragmentShaderPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexSource = vShaderStream.str();
    fragmentSource = fShaderStream.str();
  }
  catch (const std::exception&)
  {
    utils::print("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
  }
  return std::make_pair(vertexSource.c_str(), fragmentSource.c_str());
}

GLuint createShader(GLenum shaderType, const char* source)
{
  GLuint shaderId = glCreateShader(shaderType);
  glShaderSource(shaderId, 1, &source, NULL);
  glCompileShader(shaderId);

  int success;
  char infoLog[512];
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shaderId, sizeof(infoLog), NULL, infoLog);
    utils::print(infoLog);
    utils::print(source);
  }
  return shaderId;
}

GLuint createProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
  GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
  GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
    utils::print(infoLog);
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  return program;
}

Shader::Shader(const char * vertexShaderPath, const char * fragmentShaderPath)
{
  // read shader source code from file
  std::pair<std::string, std::string> shaderCodes = readShaderCodeFromFile(vertexShaderPath, fragmentShaderPath);

  // create shaders
  ID = createProgram(shaderCodes.first.c_str(), shaderCodes.second.c_str());
}

void Shader::use()
{
  glUseProgram(ID);
}

void Shader::setBool(const std::string & name, bool value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string & name, int value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string & name, float value) const
{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMatrix(const std::string & name, glm::mat4 matrix)
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const std::string & name, float x, float y, float z)
{
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string & name, glm::vec3 v)
{
  setVec3(name, v.x, v.y, v.z);
}



Shader::~Shader()
{
}
