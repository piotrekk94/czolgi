#include "camera.hpp"

glm::mat4 Camera::getVMatrix(){
    return glm::lookAt(this->pos, this->pos + this->front, this->up);
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 front, float yaw, float pitch){
  this->pos = pos;
  this->up = up;
  this->front = front;
  this->yaw = yaw;
  this->pitch = pitch;
  this->right = glm::vec3(0, 0, 1);
}

void Camera::setPos(float x, float y, float z){
  this->pos = glm::vec3(x, y, z);
}
void Camera::setPos(glm::vec3 pos){
  this->pos = pos;
}

void Camera::setYaw(float yaw){
  this->yaw = yaw;
  updateVector();
}
void Camera::setPitch(float pitch){
  this->pitch = pitch;
  updateVector();
}

float Camera::getYaw(){
  return yaw;
}

float Camera::getPitch(){
  return pitch;
}

void Camera::updateVector(){
      glm::vec3 newFront;
      newFront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
      newFront.y = sin(glm::radians(this->pitch));
      newFront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
      this->front = glm::normalize(newFront);
      this->right = glm::normalize(glm::cross(this->front, this->up));
      //this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::move(Direction direction, float deltaTime){
  switch (direction) {
    case FORWARD:
      this->pos += this->front * deltaTime;
      break;
    case BACKWARD:
      this->pos -= this->front * deltaTime;
      break;
    case LEFT:
      this->pos -= this->right * deltaTime;
      break;
    case RIGHT:
      this->pos += this->right * deltaTime;
      break;
  }
  this->pos.y = 0.0f;
}

void Camera::rotate(float dx, float dy){
  float sensitivity = 0.05f;
  dx *= sensitivity;
  dy *= sensitivity;
  this->yaw += dx;
  this->pitch += dy;
  if (this->pitch > 89.0f){
    this->pitch = 89.0f;
  }
  if (this->pitch < -89.0f){
    this->pitch = -89.0f;
  }
  updateVector();
}
