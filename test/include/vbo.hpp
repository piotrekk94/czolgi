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
VBO(ShaderProgram *shader, std::vector<glm::vec3> *points, std::vector<glm::vec2> *textureCoordinates , std::vector<glm::vec3> *normals, std::vector<GLuint> *indices = nullptr);
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
	int elementArray = 0;
	GLuint vertexArrayID;
	int readOBJ(const char *fileName,int *hasTextureCoords, unsigned *whichMesh = nullptr);
	template <class T>
	int genBuf(GLuint *buffer, std::vector<T> *arrayPtr, GLuint type = GL_ARRAY_BUFFER);
	void assignVBO(const char * name, GLuint buf, int points, GLuint type = GL_ARRAY_BUFFER);
	ShaderProgram *shader;
	int verticesAmount;
	GLuint normalsBuffer;
	GLuint tangentsBuffer;
	GLuint bitangentsBuffer;
	GLuint vertexBuffer;
	GLuint elementBuffer;

	GLuint textureBuffer; // koordynaty teksturowania
};
template <class T>
int VBO::genBuf(GLuint *buffer, std::vector<T> *vectorPtr, GLuint type)
{
	glGenBuffers(1, buffer);
	glBindBuffer(type, *buffer);
	glBufferData(type,
			vectorPtr->size() * sizeof((*vectorPtr)[0]), //rozmiar
			&((*vectorPtr)[0]), //wskaźnik
			GL_STATIC_DRAW); //
	return 0;
}
