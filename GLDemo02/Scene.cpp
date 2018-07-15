#include "Scene.h"

#include "utils.h"
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "GLApp.h"
#include <functional>

#define KEY_PRESSED(window, key) glfwGetKey(window, key) == GLFW_PRESS

Scene::Scene()
{
  OnStart();
}

void Scene::OnStart()
{
  GLApp::GetInstance()->SetFrameBufferSizeCallback(
    std::bind(&Scene::frame_buffer_size_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  GLApp::GetInstance()->SetCursorPosCallback(
    std::bind(&Scene::mouse_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  GLApp::GetInstance()->SetScrollCallback(
    std::bind(&Scene::scroll_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  // camera
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  this->camera = new Camera(cameraPos, cameraUp, yaw, pitch);

  // enable global opengl state
  glEnable(GL_DEPTH_TEST);

  float verticesFirst[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
  };
  // create shader program
  shaderCubes = new Shader("cubes.vs", "cubes.fs");
  shaderLight = new Shader("light.vs", "light.fs");
  // create vao and vbo
  GLuint vbo;
  glGenVertexArrays(1, &vaoCubes);
  glGenBuffers(1, &vbo);
  // bind vbo and set data
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFirst), verticesFirst, GL_STATIC_DRAW);
  // bind vao first
  glBindVertexArray(vaoCubes);
  // configure vertex attributes pointer
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);

  glGenVertexArrays(1, &vaoLight);
  glBindVertexArray(vaoLight);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
  glEnableVertexAttribArray(0);
}

void Scene::OnUpdate(float daltaTime)
{
  // clear screen
  glClearColor(.2f, .3f, .3f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // light
  shaderLight->use();
  shaderLight->setMatrix("projection", camera->GetProjectionMatrix());
  shaderLight->setMatrix("view", camera->GetViewMatrix());
  glm::mat4 model;
  model = glm::scale(model, glm::vec3(0.2f));
  lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
  lightPos.y = 0.5f + sin(glfwGetTime() / 2) * 1.0f;
  model = glm::translate(model, lightPos);
  shaderLight->setMatrix("model", model);
  glBindVertexArray(vaoLight);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // cubes
  shaderCubes->use();
  shaderCubes->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  shaderCubes->setVec3("lightColor", 1.0f, 0.5f, 1.0f);
  shaderCubes->setVec3("lightPos", lightPos);
  shaderCubes->setVec3("viewPos", camera->Position);
  shaderCubes->setFloat("ratio", ratio);
  shaderCubes->setMatrix("projection", camera->GetProjectionMatrix());
  shaderCubes->setMatrix("view", camera->GetViewMatrix());
  model = glm::mat4();
  model = glm::translate(model, cubePositions[9]);
  shaderCubes->setMatrix("model", model);
  glBindVertexArray(vaoCubes);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Scene::frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void Scene::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = (float)xpos;
    lastY = (float)ypos;
    firstMouse = false;
    return;
  }

  float xoffset = (float)xpos - lastX;
  float yoffset = lastY - (float)ypos;
  lastX = (float)xpos;
  lastY = (float)ypos;
  camera->ProcessRotation(xoffset, yoffset);
}

void Scene::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  camera->ProcessZoom((float)yoffset);
}

void Scene::ProcessInput(GLFWwindow* window, float deltaTime)
{
  if (KEY_PRESSED(window, GLFW_KEY_ESCAPE))
  {
    glfwSetWindowShouldClose(window, true);
  }
  if (KEY_PRESSED(window, GLFW_KEY_UP))
  {
    ratio = utils::clamp(ratio + 0.0001f, 0.0f, 1.0f);
  }
  if (KEY_PRESSED(window, GLFW_KEY_DOWN))
  {
    ratio = utils::clamp(ratio - 0.0001f, 0.0f, 1.0f);
  }
  if (KEY_PRESSED(window, GLFW_KEY_W))
  {
    camera->ProcessTranslation(FORWARD, deltaTime);
  }
  if (KEY_PRESSED(window, GLFW_KEY_S))
  {
    camera->ProcessTranslation(BACKWARD, deltaTime);
  }
  if (KEY_PRESSED(window, GLFW_KEY_A))
  {
    camera->ProcessTranslation(LEFT, deltaTime);
  }
  if (KEY_PRESSED(window, GLFW_KEY_D))
  {
    camera->ProcessTranslation(RIGHT, deltaTime);
  }
}


Scene::~Scene()
{
}
