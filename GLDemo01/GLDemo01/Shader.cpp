#include "Shader.h"



Shader::Shader(const char * vertexShaderPath, const char * fragmentShaderPath)
{
  // read shader source code from file
  const char *vertexShaderCode, *fragmentShaderCode;
  readShaderCodeFromFile(vertexShaderPath, fragmentShaderPath, vertexShaderCode, fragmentShaderCode);

  // create shaders
  ID = createProgram(vertexShaderCode, fragmentShaderCode);
}

void readShaderCodeFromFile(const char * vertexShaderPath, const char * fragmentShaderPath,
                            const char * vShaderCode, const char * fShaderCode)
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
  vShaderCode = vertexSource.c_str();
  fShaderCode = fragmentSource.c_str();
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



Shader::~Shader()
{
}
