#include "model.hpp"
int Model::draw()
{

	shader->on();
	glUniformMatrix4fv(	shader->getUniformLocation("mvp"), 1, false, glm::value_ptr(MVPMatrix));
	// Draw the triangle !
	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 4); // Starting from vertex 0; 3 vertices total -> 1 triangle
	//glDisableVertexAttribArray(0); // to usuwa
	glBindVertexArray(0);
}
Model::Model(std::vector<GLfloat> &inVertex, ShaderProgram * shader, GLuint vertexArrayID)
{
	this->shader = shader;
	this->vertex = inVertex;
	this->vertexArrayID = vertexArrayID;
	//
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(vertex[0]), &vertex[0], GL_STATIC_DRAW);
	//
	//glGenVertexArrays(1,&vertexArrayID); // ??? osobne dla każdego modelu czy nie?
	glBindVertexArray(this->vertexArrayID);
	assignVBO("vertex");
	glBindVertexArray(0);
}
int Model::setMVPMatrix(glm::mat4 MVPMatrix)
{
	this->MVPMatrix = MVPMatrix;
	return 0;
}
void Model::assignVBO(const char * name)
{
	GLuint location = shader->getAttribLocation(name);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
}
