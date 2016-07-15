#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader.hpp"
class Model
{
	public:
		int draw();
		Model(std::vector<GLfloat> &inVertex, ShaderProgram * shader, GLuint vertexArrayID);
		int setMVPMatrix(glm::mat4 MVPMatrix);
	private:
		std::vector<GLfloat> vertex;
		ShaderProgram * shader = nullptr;
		int angle_x, angle_y, angle_z;
void assignVBO(const char * name);
		GLuint vertexbuffer;
		GLuint vertexArrayID;
		glm::mat4 MMatrix = glm::mat4(1.0f);	
		glm::mat4 VMatrix = glm::mat4(1.0f);	
		glm::mat4 PMatrix = glm::mat4(1.0f);	
		glm::mat4 MVPMatrix = glm::mat4(1.0f);	

};
