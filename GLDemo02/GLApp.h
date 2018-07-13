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
  GLApp();
  int InitContext();
  int PrepareWindow();
  void SetFrameBufferSizeCallback(std::binary_function<>);
  void SetCursorPosCallback(GLFWcursorposfun cbfun);
  void SetScrollCallback(GLFWscrollfun cbfun);
  void LoadScene(Scene* scene);
  int Loop();
  ~GLApp();

private:
  GLFWwindow * window;
  Scene * curScene;
  // timing
  float lastFrame = 0.0f;
  float deltaTime = 0.0f;
  void processInput(GLFWwindow* window, float deltaTime);
};

#endif // !GLAPP_H
