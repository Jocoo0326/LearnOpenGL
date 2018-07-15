#pragma once
#ifndef GLAPP_H
#define GLAPP_H

#include "Scene.h"
#include "config.h"
#include <GLFW/glfw3.h>
#include <functional>

class GLApp
{
public:
  static GLApp * GetInstance();
  int InitContext();
  int PrepareWindow();
  void SetFrameBufferSizeCallback(std::function<void(GLFWwindow*, int, int)>const & cb);
  void SetCursorPosCallback(std::function<void(GLFWwindow*, double, double)>const & cb);
  void SetScrollCallback(std::function<void(GLFWwindow*, double, double)>const & cb);
  void LoadScene(Scene* scene);
  int Loop();
  ~GLApp();
  // callbacks
  std::function<void(GLFWwindow*, int, int)> frameBufferSizeCB;
  std::function<void(GLFWwindow*, double, double)> cursorPosCB;
  std::function<void(GLFWwindow*, double, double)> scrollCB;

private:
  static GLApp* gApp;
  GLApp();
  GLFWwindow* window;
  Scene* curScene;
  // timing
  float lastFrame = 0.0f;
  float deltaTime = 0.0f;
  void processInput(GLFWwindow* window, float deltaTime);

};

#endif // !GLAPP_H
