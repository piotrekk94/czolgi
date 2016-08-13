#include "model.hpp"
#include "shader.hpp"
#include "camera.hpp"
#define KADLUB 0
#define GASIENICA0 1
#define GASIENICA1 2
#define WIEZA 3
#define LUFA 4
#define TANK_SCALE 0.04f


class Tank
{
public:
  Tank(const char *fileName, ShaderProgram *shader, Camera *camera);
  void move(Direction direction, float deltaTime);
  void draw();
  void updateCamera();
  void toggleFirstPerson();
private:
  bool firstPerson = false;
  ShaderProgram *shader;
  glm::vec3 position, hullFront, turretFront;
  float hullRoatation, turretRotation;
  std::vector<Model> parts;
  Camera *camera;
  void updateTurret();
  void updateHull();
};
