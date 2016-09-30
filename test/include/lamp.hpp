#pragma once
#include "model.hpp"

class Lamp
{
	public:
Lamp(ShaderProgram * shader, glm::vec4 position);
void draw();
std::vector<Model> parts;
	protected:
Light light;
};
