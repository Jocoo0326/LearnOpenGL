#include "Scene.h"

#include "utils.h"
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <functional>

#define KEY_PRESSED(window, key) glfwGetKey(window, key) == GLFW_PRESS

Scene::Scene(GLApp* glapp)
{
  app = glapp;
  OnStart();
}

void Scene::OnStart()
{
  app->SetFrameBufferSizeCallback(std::bind(this->frame_buffer_size_callback, this));
  app->SetCursorPosCallback(this->mouse_callback);
  app->SetScrollCallback(this->scroll_callback);

  // camera
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  this->camera = new Camera(cameraPos, cameraUp, yaw, pitch);

  // enable global opengl state
  glEnable(GL_DEPTH_TEST);

  float verticesFirst[] = {
    //// position         // color          // texture position
    // 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    // 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    //-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    //-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
  };
  // create shader program
  shaderCubes = new Shader("vertex.vs", "fragment.fs");
  // create vao and vbo
  GLuint vbo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  //glGenBuffers(1, &ebo);
  // bind vao first
  glBindVertexArray(vao);
  // bind vbo and set data
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFirst), verticesFirst, GL_STATIC_DRAW);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // configure vertex attributes pointer
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
  glEnableVertexAttribArray(0);
  //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
  //glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(2);

  // create texture 0
  glGenTextures(1, &textureID0);
  glBindTexture(GL_TEXTURE_2D, textureID0);
  // set up texture wrap and filter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load image
  int width0, height0, nrChannels0;
  unsigned char* data0 = stbi_load("container.jpg", &width0, &height0, &nrChannels0, 0);
  if (data0)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width0, height0, 0, GL_RGB, GL_UNSIGNED_BYTE, data0);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    utils::print("Failed to load texture");
  }
  stbi_image_free(data0);
  // create texture 1
  glGenTextures(1, &textureID1);
  glBindTexture(GL_TEXTURE_2D, textureID1);
  // set up texture wrap and filter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // load image
  int width1, height1, nrChannels1;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data1 = stbi_load("awesomeface.png", &width1, &height1, &nrChannels1, 0);
  if (data1)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    utils::print("Failed to load texture");
  }
  stbi_image_free(data1);

  shaderCubes->use();
  shaderCubes->setInt("texture0", 0);
  shaderCubes->setInt("texture1", 1);
}

void Scene::OnUpdate(float daltaTime)
{
  // clear screen
  glClearColor(.2f, .3f, .3f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textureID1);

  shaderCubes->use();
  shaderCubes->setFloat("ratio", ratio);

  // set up projection matrix which rarely changes
  glm::mat4 projection = camera->GetProjectionMatrix();
  shaderCubes->setMatrix("projection", projection);

  // set up view matrix
  glm::mat4 view = camera->GetViewMatrix();
  shaderCubes->setMatrix("view", view);

  glBindVertexArray(vao);
  for (size_t i = 0; i < 10; i++)
  {
    glm::mat4 model;
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i;
    if (i % 3 == 0)
    {
      angle = (float)glfwGetTime() * 25.0f;
    }
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    shaderCubes->setMatrix("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


  //trans = glm::mat4();
  //float sc = glm::abs(glm::sin(glfwGetTime()));
  //trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
  //trans = glm::scale(trans, glm::vec3(sc, sc, 0.0f));
  //shader.setMatrix("trans", trans);
  //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
