#pragma once
#include <glm/gtx/transform.hpp>

enum Direction {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

class Camera
{
public:
  Camera(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0), glm::vec3 front = glm::vec3(0, 0, -1), float yaw = 0, float pitch = 0);
  glm::mat4 getVMatrix();

  void setPos(float x, float y, float z);
  void setPos(glm::vec3 pos);

  void setYaw(float yaw);
  void setPitch(float pitch);

  float getYaw();
  float getPitch();

  void move(Direction direction, float deltaTime);
  void rotate(float dx, float dy);
private:
  glm::vec3 pos, front, up, right;
  float yaw, pitch;
  void updateVector();
};
