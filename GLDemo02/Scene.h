#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "GLApp.h"
#include "config.h"
#include <GLFW/glfw3.h>
#include "Shader.h"

class Scene
{
public:
  Scene(GLApp* glapp);
  void OnStart();
  void OnUpdate(float daltaTime);
  void ProcessInput(GLFWwindow* window, float deltaTime);
  ~Scene();

private:
  GLApp * app;
  Camera * camera;
  // Euler angles
  float yaw = -90.0f;
  float pitch = 0.0f;
  // cursor position
  bool firstMouse = true;
  float lastX = WIDTH / 2.0f;
  float lastY = HEIGHT / 2.0f;
  // zoom ratio
  float ratio;
  // window callbacks
  void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
  void mouse_callback(GLFWwindow *window, double xpos, double ypos);
  void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

  GLuint textureID0;
  GLuint textureID1;
  Shader* shaderCubes;
  GLuint vao;
  glm::vec3 cubePositions[10] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };
};

#endif // !SCENE_H