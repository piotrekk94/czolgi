#include "model.hpp"
int Model::draw()
{

	shader->on();
	glUniformMatrix4fv(	shader->getUniformLocation("M"), 1, false, glm::value_ptr(MMatrix));
	glUniformMatrix4fv(	shader->getUniformLocation("V"), 1, false, glm::value_ptr(VMatrix));
	glUniformMatrix4fv(	shader->getUniformLocation("P"), 1, false, glm::value_ptr(PMatrix));
	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, verticesAmount ); // Starting from vertex 0; 3 vertices total -> 1 triangle
	//glDisableVertexAttribArray(0); // to usuwa
	glBindVertexArray(0);
}
Model::Model(std::vector<GLfloat> &inVertex, ShaderProgram * shader, GLuint vertexArrayID)
{
	this->shader = shader;
	//this->vertex = inVertex;
	glGenVertexArrays(1,&vertexArrayID); // ??? osobne dla każdego modelu czy nie?
	this->vertexArrayID = vertexArrayID;
	verticesAmount = inVertex.size() / 3;
	//
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, verticesAmount * 3 * sizeof(inVertex[0]), &inVertex[0], GL_STATIC_DRAW);
	
	//
	glBindVertexArray(this->vertexArrayID);
	assignVBO("vertex", vertexbuffer);
	glBindVertexArray(0);
}
Model::Model(const char * fileName, ShaderProgram * shader, GLuint vertexArrayID)
{
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> texture;
	std::vector<GLfloat> normals;
	int totalVertices = 0;
	Assimp::Importer importer;
	glGenVertexArrays(1,&vertexArrayID); // ??? osobne dla każdego modelu czy nie?
	this->shader = shader;
	this->vertexArrayID = vertexArrayID;
	const aiScene * scene = importer.ReadFile( fileName, aiProcess_Triangulate);
	if (!scene)
	{
		fprintf(stderr,"Błąd odczytu: %s\n", fileName);
	}
	for(int i = 0 ; i < scene->mNumMeshes ; i++) //!!! wczytywanie wszytkich czesci na raz
	{
		aiMesh * mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;
		for ( int j = 0 ; j < iMeshFaces ; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			for(int k = 0 ; k < 3 ; k++)
			{
				aiVector3t<float> pos = mesh->mVertices[face.mIndices[k]];

				vertices.push_back(pos[0]);
				vertices.push_back(pos[1]);
				vertices.push_back(pos[2]);
			}
			if (mesh->HasTextureCoords(0))
			{
				for(int k = 0 ; k < 3 ; k++)
				{
					aiVector3t<float> uv = mesh->mTextureCoords[0][face.mIndices[k]];
					texture.push_back(uv[0]);
					texture.push_back(uv[1]);
				}
			}
			else
				fprintf(stderr,"plik nie współrzędnych teksturowania: %s\n", fileName);
			if (mesh->HasNormals())
			{
				for(int k = 0 ; k < 3 ; k++)
				{
					aiVector3t<float> temp = mesh->mNormals[face.mIndices[k]];
					normals.push_back(temp[0]);
					normals.push_back(temp[1]);
					normals.push_back(temp[2]);
				}
			}
			else fprintf(stderr,"plik nie ma normalnych: %s\n", fileName);
		}
		totalVertices += mesh->mNumVertices;
	}
	//
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, texture.size() * sizeof(texture[0]), &texture[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
	//
	//glGenVertexArrays(1,&vertexArrayID); // ??? osobne dla każdego modelu czy nie?
	glBindVertexArray(this->vertexArrayID);
	assignVBO("vertex", vertexbuffer);
	assignVBO("Normals", normalsBuffer);
	assignVBO("vertexTexture", textureBuffer);
	glBindVertexArray(0);
	verticesAmount = totalVertices;

}
Model::~Model()
{
	glDeleteTextures(1,&texture);
}
void Model::textureLoad(const char * fileName)
{
	int width, height, channels;
	unsigned char * image = SOIL_load_image(fileName, &width, &height, &channels,  SOIL_LOAD_RGB);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
// 	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
int Model::setMVPMatrix(glm::mat4 MVPMatrix)
{
	this->MVPMatrix = MVPMatrix;
	return 0;
}
int Model::setMMatrix(glm::mat4 MMatrix)
{
	this->MMatrix = MMatrix;
	return 0;
}
int Model::setVMatrix(glm::mat4 VMatrix)
{
	this->VMatrix = VMatrix;
	return 0;
}
int Model::setPMatrix(glm::mat4 PMatrix)
{
	this->PMatrix = PMatrix;
	return 0;
}
void Model::translate(glm::vec3 vector)
{
	MMatrix = glm::translate(MMatrix, vector);
}
void Model::scale(glm::vec3 vector)
{
	MMatrix = glm::scale(MMatrix, vector);
}
void Model::rotate(float angle, glm::vec3 vector)
{
	MMatrix = glm::rotate(MMatrix, angle, vector);
}
void Model::assignVBO(const char * name, GLuint buf)
{
	int points = 3;
	if (!strcmp(name,"vertexTexture")) points = 2;
	GLuint location = shader->getAttribLocation(name);
	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glEnableVertexAttribArray(location);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
			location,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			points,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
}
