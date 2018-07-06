#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utils.h"
#include "config.h"

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

void doRender()
{
}

static float verticesFirst[] = {
  // position         // color
  -0.9f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // left
  -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right
  -0.7f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // top

   //0.5f, -0.5f, 0.0f, // left
   //0.0f,  0.5f, 0.0f, // right
   //0.5f,  0.5f, 0.0f, // top
};

static float verticesSecond[] = {
   0.5f, -0.5f, 0.0f, // left
   0.0f,  0.5f, 0.0f, // right
   0.5f,  0.5f, 0.0f, // top
};

static const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec4 vertexColor;\n"
"void main() {\n"
"  gl_Position = vec4(aPos, 1.0f);\n"
"  vertexColor = vec4(aColor, 1.0f);\n"
"}\n";

static const GLchar* fragmentShaderSource =
"#version 330 core\n"
"in vec4 vertexColor;\n"
"out vec4 FragColor;\n"
//"uniform vec4 ourColor;\n"
"void main() {\n"
"  FragColor = vertexColor;\n"
"}\n";

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

GLuint createProgram()
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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
  // glfw: initialize glfw and configure
  // -----------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", NULL, NULL);
  if (!window)
  {
    utils::print("Failed to create GLFW window");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

  // glad: load all OpenGL function pointers 
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    utils::print("Failed to init glad");
    return -1;
  }

  // create shader program
  GLuint program = createProgram();
  // create vao and vbo
  GLuint vao[2], vbo[2];
  glGenVertexArrays(2, vao);
  glGenBuffers(2, vbo);
  // bind vao first
  glBindVertexArray(vao[0]);
  // bind vbo and set data
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFirst), verticesFirst, GL_STATIC_DRAW);
  // configure vertex attributes pointer
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);

  // bind vao first
  glBindVertexArray(vao[1]);
  // bind vbo and set data
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSecond), verticesSecond, GL_STATIC_DRAW);
  // configure vertex attributes pointer
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
  glEnableVertexAttribArray(0);

  // render loop
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    doRender();
    // clear screen
    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // uniform pass data from cpu to gpu
    //double timeValue = glfwGetTime();
    //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    //GLint vertexColorLocation = glGetUniformLocation(program, "ourColor");
    //glUseProgram(program);
    //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    glUseProgram(program);
    glBindVertexArray(vao[0]); 
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(vao[1]); 
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // unbind vbo
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // unbind vao
  glBindVertexArray(0);

  glfwTerminate();
  return 0;
}
