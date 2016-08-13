#include "tank.hpp"

Tank::Tank(const char *fileName, ShaderProgram *shader, Camera *camera){
  for (int i = 0; i < 5; i++){
    unsigned j = i;
    parts.push_back(Model(fileName, shader, &j));
    parts[i].setScale(TANK_SCALE, TANK_SCALE, TANK_SCALE);
  }
  this->camera = camera;
  position = glm::vec3(0.0f, 0.0f, 0.0f);
  front = glm::vec3(0.0f, 0.0f, 1.0f);
}

void Tank::move(Direction direction, float deltaTime){
  switch (direction) {
    case FORWARD:
      position += front * deltaTime;
      break;
    case BACKWARD:
      position -= front * deltaTime;
      break;
    case LEFT:
      hullRoatation -= deltaTime * 1.0f;
      break;
    case RIGHT:
      hullRoatation += deltaTime * 1.0f;
      break;
  }
  updateVector();
}

void Tank::draw(){
  turretRotation = glm::radians(camera->getYaw());
  parts[KADLUB].setAngle(0, glm::radians(90.0f) - hullRoatation, 0);
  parts[GASIENICA0].setAngle(0, glm::radians(90.0f) - hullRoatation, 0);
  parts[GASIENICA1].setAngle(0, glm::radians(90.0f) - hullRoatation, 0);
  parts[WIEZA].setAngle(0, glm::radians(90.0f) - turretRotation, 0);
  parts[LUFA].setAngle(0, glm::radians(90.0f) - turretRotation, 0);
  for (unsigned i = 0; i < parts.size(); i++){
    parts[i].setPos(position.x, position.y, position.z);
    parts[i].draw();
  }
}

void Tank::updateCamera(){
  camera->setPos(position.x, position.y + 10*TANK_SCALE, position.z);
}

void Tank::updateVector(){
  glm::vec3 newFront;
  newFront.x = cos(hullRoatation);
  newFront.y = 0;
  newFront.z = sin(hullRoatation);
  front = glm::normalize(newFront);
}
