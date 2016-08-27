#pragma once

#include <glm/gtx/transform.hpp>

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"

class VBO
{
	public:
	VBO(ShaderProgram *shader, const char *fileName, int *hasTextureCoords, unsigned *whichMesh = nullptr);
//	~VBO();
	void activate(){
		glBindVertexArray(vertexArrayID);
	};
	void deactivate(){
		glBindVertexArray(0);
	};
	int getVerticesAmount(){
		return verticesAmount;
	};
	private:
	GLuint vertexArrayID;
	int readOBJ(const char *fileName,int *hasTextureCoords, unsigned *whichMesh = nullptr);
	int genBuf(GLuint *buffer, std::vector<glm::vec3> *arrayPtr);
	void assignVBO(const char * name, GLuint buf, int points);
	ShaderProgram *shader;
	int verticesAmount;
	GLuint normalsBuffer;
	GLuint tangentsBuffer;
	GLuint bitangentsBuffer;
	GLuint vertexbuffer;

	GLuint textureBuffer; // koordynaty teksturowania
};
