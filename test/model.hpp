#pragma once
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>

#include "shader.hpp"

struct Light
{
	glm::vec4 position;
	glm::vec3 color = glm::vec3(1,1,1);
	GLfloat power = 200;
	GLint type = 1; // 1 - punktowe;
/*	Light(glm::vec4 p, glm::vec3 color = glm::vec3(1,1,1))
	{
		position = p;
		this -> color = color;
	}*/
};

class Model
{
	public:
		int draw();
		Model(std::vector<GLfloat> &inVertex, ShaderProgram *shader, GLuint vertexArrayID);
		Model(const char *fileName, ShaderProgram *shader, GLuint vertexArrayID, unsigned *whichMesh = nullptr);
		~Model();
		int setMVPMatrix(glm::mat4 MVPMatrix);
		int setVMatrix(glm::mat4 MMatrix);
		int setMMatrix(glm::mat4 MMatrix);
		int setPMatrix(glm::mat4 MMatrix);
		int textureLoad(const char *fileName);
		void resetMMatrix();
		void rotate(float angle, glm::vec3 vector);
		void translate(glm::vec3 vector);
		void scale(glm::vec3 vector);
		static std::vector<Light> light;
	private:
		int enableLight();
		//std::vector<GLfloat> vertex;
		int verticesAmount;
		ShaderProgram *shader = nullptr;
		int angle_x, angle_y, angle_z;
		GLint hasTexture = 0;
		GLint hasTextureCoords = 0;
		void assignVBO(const char * name, GLuint buf, int points);
		GLuint vertexbuffer;
		GLuint textureBuffer;
		GLuint normalsBuffer;
		GLuint lightBuffer;
		GLuint texture;
		GLuint vertexArrayID;
		glm::mat4 MMatrix = glm::mat4(1.0f);
		glm::mat4 VMatrix = glm::mat4(1.0f);
		glm::mat4 PMatrix = glm::mat4(1.0f);
		glm::mat4 MVPMatrix = glm::mat4(1.0f);
};
