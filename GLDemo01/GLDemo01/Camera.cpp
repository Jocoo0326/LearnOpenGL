#include "Camera.h"

#include "utils.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, Camera_Projection mode)
  : MovementSpeed(CAMERA_SPEED_COEFFICIENT), MouseSensitivity(ROTATION_SENSITIVITY), Zoom(MAX_FOV), Mode(PERSPECTIVE)
{
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  Mode = mode;
  updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, Camera_Projection mode)
  : MovementSpeed(CAMERA_SPEED_COEFFICIENT), MouseSensitivity(ROTATION_SENSITIVITY), Zoom(MAX_FOV), Mode(PERSPECTIVE)
{
  Position = glm::vec3(posX, posY, posZ);
  WorldUp = glm::vec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  Mode = mode;
  updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
  return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
  if (Mode == PERSPECTIVE)
  {
    return glm::perspective(glm::radians(Zoom), 1.0f * WIDTH / HEIGHT, 0.1f, 100.f);
  }
  else
  {
    return glm::ortho(0.0f, WIDTH * 1.0f, HEIGHT * 1.0f, 0.0f);
  }
}

void Camera::ProcessTranslation(Camera_Movement_Direction direction, float deltaTime)
{
  float velocity = CAMERA_SPEED_COEFFICIENT * deltaTime;
  switch (direction)
  {
  case FORWARD:
    Position += velocity * Front;
    break;
  case BACKWARD:
    Position -= velocity * Front;
    break;
  case LEFT:
    Position -= velocity * Right;
    break;
  case RIGHT:
    Position += velocity * Right;
    break;
  default:
    break;
  }
  Position.y = 0;
}

void Camera::ProcessRotation(float xoffset, float yoffset, GLboolean constrianPitch)
{
  xoffset *= ROTATION_SENSITIVITY;
  yoffset *= ROTATION_SENSITIVITY;

  Yaw += xoffset;
  Pitch += yoffset;

  if (constrianPitch)
  {
    Pitch = utils::clamp(Pitch, MIN_PITCH, MAX_PITCH);
  }
  updateCameraVectors();
}


void Camera::ProcessZoom(float offset)
{
  if (Zoom >= MIN_FOV && Zoom <= MAX_FOV)
  {
    Zoom -= offset;
  }
  Zoom = utils::clamp(Zoom, MIN_FOV, MAX_FOV);
}

Camera::~Camera()
{
}

void Camera::updateCameraVectors()
{
  glm::vec3 front;
  front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
  front.y = sin(glm::radians(Pitch));
  front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
  Front = glm::normalize(front);

  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::cross(Right, Front);
}
