#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
//assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>
//

#include "shader.hpp"
class Model
{
	public:
		int draw();
		Model(std::vector<GLfloat> &inVertex, ShaderProgram * shader, GLuint vertexArrayID);
		Model(const char * fileName, ShaderProgram * shader, GLuint vertexArrayID);
		~Model();
		int setMVPMatrix(glm::mat4 MVPMatrix);
int setVMatrix(glm::mat4 MMatrix);
int setMMatrix(glm::mat4 MMatrix);
int setPMatrix(glm::mat4 MMatrix);
		void textureLoad(const char * fileName);
		void resetMMatrix();
		void rotate(float angle, glm::vec3 vector);
		void translate(glm::vec3 vector);
		void scale(glm::vec3 vector);
	private:
//		std::vector<GLfloat> vertex;
		int verticesAmount;
		ShaderProgram * shader = nullptr;
		int angle_x, angle_y, angle_z;
void assignVBO(const char * name, GLuint buf);
		GLuint vertexbuffer;
		GLuint textureBuffer;
		GLuint normalsBuffer;
		GLuint texture;
		GLuint vertexArrayID;
		glm::mat4 MMatrix = glm::mat4(1.0f);	
		glm::mat4 VMatrix = glm::mat4(1.0f);	
		glm::mat4 PMatrix = glm::mat4(1.0f);	
		glm::mat4 MVPMatrix = glm::mat4(1.0f);	

};
