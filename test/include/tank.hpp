#pragma once
#include "model.hpp"
#include "shader.hpp"
#include "camera.hpp"

#define KADLUB 0
#define GASIENICA0 1
#define GASIENICA1 2
#define WIEZA 3
#define LUFA 4
#define TANK_SCALE 0.04f
#define ACCELERATION 0.1f
#define DECELERATION 0.2f
#define ROTATION 0.5f
#define TOP_SPEED 0.7f


class Tank
{
public:
  Tank(const char *fileName, ShaderProgram *shader, Camera *camera);
  void handleKeys(Direction direction, float deltaTime);
  void move(float deltaTime);
  void draw(glm::vec3 normal);
  void updateCamera();
  void updateTurret(float turretAngle);
  void toggleFirstPerson();
  void textureLoad(const char * fileName);
  void setHeight(float y){position.y = y;};
  float getX(){return position.x;};
  float getY(){return position.y;};
  float getZ(){return position.z;};
  void rotate(glm::vec3 normal);
private:
  float speed = 0;
  bool firstPerson = false;
  bool accelerated = false;
  ShaderProgram *shader;
  glm::vec3 position, hullFront, turretFront;
  float hullRoatation, turretRotation, gunRotation;
  std::vector<Model> parts;
  Camera *camera;
  void updateTurret();
  void updateHull();
};
