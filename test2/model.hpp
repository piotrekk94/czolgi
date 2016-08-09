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
	GLfloat power = 1;
	GLint type = 1; // 1 - punktowe;
	Light(glm::vec4 p, glm::vec3 color = glm::vec3(1,1,1))
	{
		position = p;
		this -> color = color;
	}
};

class Model
{
	public:

		static int setVMatrix(glm::mat4 MMatrix);
		static int setPMatrix(glm::mat4 MMatrix);

		void setPos(float x, float y, float z);
		void setAngle(float x, float y, float z);
		void setScale(float x, float y, float z);
		void setCenter(float x, float y, float z);
		void setPos(glm::vec3 pos);
		void setAngle(glm::vec3 angle);
		void setScale(glm::vec3 scale);
		void setCenter(glm::vec3 center);

		int draw();

		Model(const char *fileName, ShaderProgram *shader, unsigned *whichMesh = nullptr);
		//whichMesh zwraca ile siatek jest w pliku
		~Model();
		int setMMatrix(glm::mat4 MMatrix);

		int textureLoad(const char *fileName);
		static std::vector<Light> light;
		static float ambient;
		float shinniness = 100;
		glm::vec4 color = glm::vec4(1,1,0,0);
		glm::vec4 specularColor = glm::vec4(1,1,1,0);
	private:
		glm::vec3 pos, angle, sc, center;

		int enableLight(); //na razie nie wykorzystywana
		int sendUniformData();
		int readOBJ(const char *fileName, unsigned *whichMesh = nullptr);

		void rotate(float angle, glm::vec3 vector);
		void translate(glm::vec3 vector);
		void scale(glm::vec3 vector);

		//std::vector<GLfloat> vertex;
		int verticesAmount;
		ShaderProgram *shader = nullptr;
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
		static glm::mat4 VMatrix;
		static glm::mat4 PMatrix;
		glm::mat4 MVPMatrix = glm::mat4(1.0f);
};