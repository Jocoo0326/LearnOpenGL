#include "GLApp.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils.h"


GLApp* GLApp::gApp = NULL;

GLApp::GLApp()
{
}

GLApp * GLApp::GetInstance()
{
  if (!gApp)
  {
    gApp = new GLApp();
  }
  return gApp;
}

int GLApp::InitContext()
{
  // glfw: initialize glfw and configure
  // -----------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

  // glad: load all OpenGL function pointers 
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    utils::print("Failed to init glad");
    return -1;
  }

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return 0;
}

void GLApp::SetFrameBufferSizeCallback(std::function<void(GLFWwindow*, int, int)>const & cb)
{
  this->frameBufferSizeCB = cb;
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int w, int h) {
    if (GLApp::GetInstance()->frameBufferSizeCB)
    {
      GLApp::GetInstance()->frameBufferSizeCB(win, w, h);
    }
  });
}

void GLApp::SetCursorPosCallback(std::function<void(GLFWwindow*, double, double)>const & cb)
{
  this->cursorPosCB = cb;
  glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
    if (GLApp::GetInstance()->cursorPosCB)
    {
      GLApp::GetInstance()->cursorPosCB(window, xpos, ypos);
    }
  });
}

void GLApp::SetScrollCallback(std::function<void(GLFWwindow*, double, double)>const & cb)
{
  this->scrollCB = cb;
  glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
    if (GLApp::GetInstance()->scrollCB)
    {
      GLApp::GetInstance()->scrollCB(window, xoffset, yoffset);
    }
  });
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
  delete gApp;
}
