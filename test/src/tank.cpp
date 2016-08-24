#include "tank.hpp"

Tank::Tank(const char *fileName, ShaderProgram *shader, Camera *camera){
  for (int i = 0; i < 5; i++){
    unsigned j = i;
    parts.push_back(Model(fileName, shader, &j));
    parts[i].setScale(TANK_SCALE, TANK_SCALE, TANK_SCALE);
  }
  this->camera = camera;

  position = glm::vec3(0.0f, 0.0f, 0.0f);
  hullFront = glm::vec3(0.0f, 0.0f, 1.0f);
  turretFront = glm::vec3(0.0f, 0.0f, 1.0f);

  hullRoatation = turretRotation = gunRotation = 0;

  parts[KADLUB].textureLoad("./tekstury/hull.dds");
//  parts[WIEZA].textureLoad("./tekstury/hull.dds");
//  parts[LUFA].textureLoad("./tekstury/hull.dds");
  parts[WIEZA].textureShare(parts[KADLUB].getTexture(0));
  parts[LUFA].textureShare(parts[KADLUB].getTexture(0));
  parts[GASIENICA0].textureLoad("./tekstury/treads.png");
  parts[GASIENICA1].textureLoad("./tekstury/treads.png");
  //paweł
  //parts[KADLUB].bumpTextureLoad("./tekstury/tankNormal.png");
  //parts[WIEZA].bumpTextureLoad("./tekstury/tankNormal.png");
  //parts[LUFA].bumpTextureLoad("./tekstury/tankNormal.png");
  //
  parts[KADLUB].bumpTextureLoad("./tekstury/tankBump.dds", 1);
  parts[WIEZA].bumpTextureLoad("./tekstury/tankBump.dds", 1);
  parts[LUFA].bumpTextureLoad("./tekstury/tankBump.dds", 1);
}

void Tank::handleKeys(Direction direction, float deltaTime){
  switch (direction) {
    case FORWARD:
      speed += ACCELERATION * deltaTime;
      if (speed > TOP_SPEED) speed = TOP_SPEED;
      accelerated = true;
      break;
    case BACKWARD:
      speed -= ACCELERATION * deltaTime;
      if (speed < -TOP_SPEED)speed = -TOP_SPEED;
      accelerated = true;
      break;
    case LEFT:
      hullRoatation -= deltaTime * ROTATION;
      break;
    case RIGHT:
      hullRoatation += deltaTime * ROTATION;
      break;
  }
  updateHull();
}

void Tank::move(float deltaTime){
	//fprintf(stderr,"speed: %f", speed);
  if (accelerated){
    accelerated = false;
  } else {
    if (speed > 0){
      speed -= DECELERATION * deltaTime;
      if (speed < 0)speed = 0;
    } else {
      speed += DECELERATION * deltaTime;
      if (speed > 0) speed = 0;
    }
  }
  position += hullFront * speed * deltaTime;
}

void Tank::draw(){
  parts[KADLUB].setAngle(0, glm::radians(90.0f) - hullRoatation, 0);
  parts[GASIENICA0].setAngle(0, glm::radians(90.0f) - hullRoatation, 0);
  parts[GASIENICA1].setAngle(0, glm::radians(90.0f) - hullRoatation, 0);
  parts[WIEZA].setAngle(0, glm::radians(90.0f) - turretRotation, 0);
  parts[LUFA].setAngle(-gunRotation, glm::radians(90.0f) - turretRotation, 0);
  parts[LUFA].setCenter(0,0.1f*TANK_SCALE,0);
  for (unsigned i = 0; i < parts.size(); i++){
    parts[i].setPos(position.x, position.y, position.z);
    parts[i].draw();
  }
}

void Tank::updateCamera(){
  glm::vec3 camPos;
  updateTurret();
  if(firstPerson){
    camPos = position; // kamera 1 os
  }else{
    camPos = position - 0.5f * turretFront; //kamera 3 os
  }
  camera->setPos(camPos.x, camPos.y + 10*TANK_SCALE, camPos.z);
}

void Tank::toggleFirstPerson(){
  firstPerson = !firstPerson;
}

void Tank::textureLoad(const char * fileName){
  for (unsigned i = 0; i < parts.size(); i++){
    parts[i].textureLoad(fileName);
  }
}

void Tank::updateTurret(){
  turretRotation = glm::radians(camera->getYaw());
  float gunAngle = camera->getPitch();
  if (gunAngle > 7) gunAngle = 7;
  else if (gunAngle < -7) gunAngle = -7;
  gunRotation = glm::radians(gunAngle);
  glm::vec3 newFront;
  newFront.x = cos(turretRotation);
  newFront.y = 0;
  newFront.z = sin(turretRotation);
  turretFront = glm::normalize(newFront);
}

void Tank::updateHull(){
  glm::vec3 newFront;
  newFront.x = cos(hullRoatation);
  newFront.y = 0;
  newFront.z = sin(hullRoatation);
  hullFront = glm::normalize(newFront);
}
