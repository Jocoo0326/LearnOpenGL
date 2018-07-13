#include "GLApp.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "utils.h"


GLApp::GLApp()
{
}

int GLApp::InitContext()
{
  // glfw: initialize glfw and configure
  // -----------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glad: load all OpenGL function pointers 
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    utils::print("Failed to init glad");
    return -1;
  }
  return 0;
}

int GLApp::PrepareWindow()
{
  // glfw window creation
  window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", NULL, NULL);
  if (!window)
  {
    utils::print("Failed to create GLFW window");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GLApp::SetFrameBufferSizeCallback(GLFWframebuffersizefun cbfun)
{
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int, int) {
  });
}

void GLApp::SetCursorPosCallback(GLFWcursorposfun cbfun)
{
  glfwSetCursorPosCallback(window, cbfun);
}

void GLApp::SetScrollCallback(GLFWscrollfun cbfun)
{
  glfwSetScrollCallback(window, cbfun);
}

void GLApp::LoadScene(Scene * scene)
{
  this->curScene = scene;
}

int GLApp::Loop()
{
  // render loop
  while (!glfwWindowShouldClose(window))
  {
    // pre-frame time logic
    // --------------------
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // process input events
    // --------------------
    processInput(window, deltaTime);

    // update scene
    // ------------
    if (curScene)
    {
      curScene->OnUpdate(deltaTime);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  return 0;
}

void GLApp::processInput(GLFWwindow * window, float deltaTime)
{
  curScene->ProcessInput(window, deltaTime);
}

GLApp::~GLApp()
{
  glfwTerminate();
}
