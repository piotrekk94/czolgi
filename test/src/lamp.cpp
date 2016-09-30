#include "lamp.hpp"

Lamp::Lamp(ShaderProgram *shader, glm::vec4 position)
	:light(position)
{
	unsigned mesh = 0;
parts.push_back(Model("./models/untitled.dae", shader, &mesh));	
		mesh = 1;
parts.push_back(Model("./models/untitled.dae", shader, &mesh));	
		mesh = 2;
parts.push_back(Model("./models/untitled.dae", shader, &mesh));
parts[0].textureLoad("./tekstury/MetalBase0123_S.jpg");
parts[1].textureLoad("./tekstury/MetalBase0123_S.jpg");
parts[2].textureLoad("./tekstury/Fiberglass0014_S.jpg");
for(unsigned i=0; i < parts.size() ; i++)
{
	parts[i].setPos(position.x,position.y -0.2,position.z);
	parts[i].setScale(20,20,20);
}
light.position.y = position.y + 1.4;
Model::light.push_back(light);
}
void Lamp::draw()
{
for(unsigned i=0; i < parts.size() ; i++)
{
	parts[i].draw();
}
}
