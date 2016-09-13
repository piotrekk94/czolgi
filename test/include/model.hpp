#pragma once
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#include <ctime>
#include <memory> // shared_ptr

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>

#include "shader.hpp"
#include "texture.hpp"
#include "vbo.hpp"
#define DEBUG 1
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

		virtual int draw();

		Model(const char *fileName, ShaderProgram *shader, unsigned *whichMesh = nullptr);
		//whichMesh zwraca ile siatek jest w pliku
		Model(ShaderProgram *shader, std::vector<glm::vec3> *points, std::vector<glm::vec2> *textureCoordinates , std::vector<glm::vec3> *normals);
		~Model();

		int setMMatrix(glm::mat4 MMatrix);

		std::shared_ptr<Texture> textureLoad(const char *fileName, int number = -1);
		std::shared_ptr<Texture> bumpTextureLoad(const char *fileName, int number = -1);
		int textureShare(const std::shared_ptr<Texture> *ptr, int number = -1);
		int bumpTextureShare(const std::shared_ptr<Texture> *ptr, int number = -1);
		const std::shared_ptr<Texture> *getTexture(int number){return &textures[number];};

		static std::vector<Light> light;
		static float ambient;
		float shinniness = 10;
		glm::vec4 color = glm::vec4(1,1,0,0);
		glm::vec4 specularColor = glm::vec4(1,1,1,0);
	protected:
		Model(){};
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
		std::shared_ptr<VBO> vbo;
		GLint hasTexture = 0;
		GLint hasBump = 0;
		GLint hasTextureCoords = 0;
		void assignVBO(const char * name, GLuint buf, int points);
		//GLuint vertexbuffer;

		//GLuint textureBuffer; // koordynaty teksturowania
		//std::vector<int> textureNumber;
		//static int globalTextureNumber;
		//std::vector<GLuint> texture;

		//
		std::vector<std::shared_ptr<Texture>> textures;
		//
		//GLuint normalsBuffer;
		//GLuint tangentsBuffer;
		//GLuint bitangentsBuffer;
		GLuint lightBuffer;
		//GLuint vertexArrayID;
		glm::mat4 MMatrix = glm::mat4(1.0f);
		static glm::mat4 VMatrix;
		static glm::mat4 PMatrix;
		glm::mat4 MVPMatrix = glm::mat4(1.0f);
};
