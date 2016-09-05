#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#include <ctime>
#include "shader.hpp"
#include "model.hpp"

#define DEBUG 1

class Terrain: public Model
{
	public:

		virtual int draw();
		Terrain(ShaderProgram *ptrShader, const char *fileName);
		float getHeight(float x, float z);
	private:
		int calculateIndices();
		int rows, columns, channels;
		int calculateNormals();
		int loadHeightMap(const char *fileName);
		std::vector<unsigned> indices;
		std::vector<glm::vec3> points;
		std::vector<glm::vec2> textureCoordinates;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> normalsToPoint;
		void assignVBO(const char * name, GLuint buf, int points);
	float Scale;
};
