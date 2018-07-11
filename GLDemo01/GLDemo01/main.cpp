#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utils.h"
#include "config.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

static float ratio;
#define KEY_PRESSED(window, key) glfwGetKey(window, key) == GLFW_PRESS
#define CAMERA_SPEED_COEFFICIENT 2.5f

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
float fov = 45.0f;
Camera camera(cameraPos, cameraUp, yaw, pitch);

// timing
float lastFrame = 0.0f;
float deltaTime = 0.0f;

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
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
  camera.ProcessRotation(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  camera.ProcessZoom((float)yoffset);
}

void processInput(GLFWwindow* window)
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
    camera.ProcessTranslation(FORWARD, deltaTime);
  }
  if (KEY_PRESSED(window, GLFW_KEY_S))
  {
    camera.ProcessTranslation(BACKWARD, deltaTime);
  }
  if (KEY_PRESSED(window, GLFW_KEY_A))
  {
    camera.ProcessTranslation(LEFT, deltaTime);
  }
  if (KEY_PRESSED(window, GLFW_KEY_D))
  {
    camera.ProcessTranslation(RIGHT, deltaTime);
  }
}

void doRender()
{
}


static float verticesFirst[] = {
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

static unsigned int indices[] = {
   0, 1, 3,
   1, 2, 3
};

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
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers 
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    utils::print("Failed to init glad");
    return -1;
  }


  // enable global opengl state
  glEnable(GL_DEPTH_TEST);

  // create shader program
  Shader shader("vertex.vs", "fragment.fs");
  // create vao and vbo
  GLuint vao, vbo;
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
  GLuint textureID0;
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
  GLuint textureID1;
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

  shader.use();
  shader.setInt("texture0", 0);
  shader.setInt("texture1", 1);

  glm::vec3 cubePositions[] = {
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
    processInput(window);

    doRender();
    // clear screen
    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureID1);

    shader.use();
    shader.setFloat("ratio", ratio);

    // set up projection matrix which rarely changes
    glm::mat4 projection = camera.GetProjectionMatrix();
    shader.setMatrix("projection", projection);

    // set up view matrix
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMatrix("view", view);

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
      shader.setMatrix("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    //trans = glm::mat4();
    //float sc = glm::abs(glm::sin(glfwGetTime()));
    //trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
    //trans = glm::scale(trans, glm::vec3(sc, sc, 0.0f));
    //shader.setMatrix("trans", trans);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
