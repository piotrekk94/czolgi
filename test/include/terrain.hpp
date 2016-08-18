#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#include <ctime>
#include "shader.hpp"
#define DEBUG 1
class Terrain
{
	public:

		int draw();
		float shinniness = 10;
		glm::vec4 color = glm::vec4(1,1,0,0);
		glm::vec4 specularColor = glm::vec4(1,1,1,0);
	private:
		ShaderProgram *shader = nullptr;
		void assignVBO(const char * name, GLuint buf, int points);
		GLuint texture;
		GLuint vertexArrayID;
};
