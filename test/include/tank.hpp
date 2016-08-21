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
  void draw();
  void updateCamera();
  void toggleFirstPerson();
  void textureLoad(const char * fileName);
  float speed = 0;
private:
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
