#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

const float CAMERA_SPEED_COEFFICIENT = 2.5f;
const float MIN_FOV = 1.0f;
const float MAX_FOV = 45.0f;
const float ROTATION_SENSITIVITY = 0.05f;
const float YAW = -90.f;
const float PITCH = 0.0f;
const float MIN_PITCH = -89.0f;
const float MAX_PITCH = 89.0f;

// Define several possible options for camera movement 
enum Camera_Movement_Direction {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// Camera projection mode
enum Camera_Projection {
  ORTHOGRAPHIC,
  PERSPECTIVE
};

class Camera
{
public:
  // Cameta attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // Euler angles
  float Yaw;
  float Pitch;
  // Projection mode
  Camera_Projection Mode;
  // Camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  // Constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = YAW, float pitch = PITCH, Camera_Projection mode = PERSPECTIVE);

  // Constructor with scalar values
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, Camera_Projection mode = PERSPECTIVE);

  // Reture the view matrix calculated using Euler angles and the LookAt Matrix
  glm::mat4 GetViewMatrix();

  // Return the projection matrix
  glm::mat4 GetProjectionMatrix();

  // Process camera translation (constriant to ground directions, left, right, forward, backward)
  void ProcessTranslation(Camera_Movement_Direction direction, float deltaTime);

  // Process rotation with Euler angles
  void ProcessRotation(float xoffset, float yoffset, GLboolean constrianPitch = true);

  // Process zoom which updates fov (field of view)
  void ProcessZoom(float offset);

  ~Camera();

private:
  void updateCameraVectors();
};

#endif // !CAMERA_H
