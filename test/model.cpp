#include "model.hpp"
std::vector<Light> Model::light;
glm::mat4 Model::VMatrix = glm::mat4(1.0f);
glm::mat4 Model::PMatrix = glm::mat4(1.0f);
float Model::ambient;
int Model::draw()
{

	shader->on();
	glUniformMatrix4fv(	shader->getUniformLocation("M"), 1, false, glm::value_ptr(MMatrix));
	glUniformMatrix4fv(	shader->getUniformLocation("V"), 1, false, glm::value_ptr(VMatrix));
	glUniformMatrix4fv(	shader->getUniformLocation("P"), 1, false, glm::value_ptr(PMatrix));
	glUniform1i(shader->getUniformLocation("hasTexture"), hasTexture);
	glUniform1f(shader->getUniformLocation("ambient"), ambient);
	glUniform1f(shader->getUniformLocation("shinniness"), shinniness);
	glUniform4f( shader->getUniformLocation("color"), color.r, color.g, color.b, color.a);
	glUniform4f( shader->getUniformLocation("specularColor"), specularColor.r, specularColor.g, specularColor.b, specularColor.a);
	//
	glUniform1i(shader->getUniformLocation("lightNumber"), light.size());
	for (unsigned int i = 0 ; i < light.size() ; i++)
	{
		std::ostringstream oss;
		oss << "light[" << i;
		glUniform4f( shader->getUniformLocation(oss.str() + "].position"), light[i].position.x, light[i].position.y, light[i].position.z, light[i].position.w);
		glUniform3f( shader->getUniformLocation(oss.str() + "].color"), light[i].color.r, light[i].color.g, light[i].color.b);
		glUniform1f( shader->getUniformLocation(oss.str() + "].power"), light[i].power);
		glUniform1i( shader->getUniformLocation(oss.str() + "].type"), light[i].type);
		
	}
	//
	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, verticesAmount ); // Starting from vertex 0; 3 vertices total -> 1 triangle
	//glDisableVertexAttribArray(0); // to usuwa
	glBindVertexArray(0);
	return 0;
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
	assignVBO("vertex", vertexbuffer, 3);
	glBindVertexArray(0);
}
int Model::enableLight()
{
	glUniformBlockBinding(shader->getID(), shader->getUniformBlockIndex("LightBlock"), 1);//???
	glGenBuffers(1, &lightBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(light[0]), &light[0],GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightBuffer);
	return 0;
}
Model::Model(const char * fileName, ShaderProgram * shader, GLuint vertexArrayID, unsigned *whichMesh)
{
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> texture;
	std::vector<GLfloat> normals;
	int totalVertices = 0;
	Assimp::Importer importer;
	this->shader = shader;
	this->vertexArrayID = vertexArrayID;
	glGenVertexArrays(1,&(this->vertexArrayID)); // ??? osobne dla każdego modelu czy nie?
	const aiScene * scene = importer.ReadFile( fileName, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FixInfacingNormals);
	if (!scene)
	{
		fprintf(stderr,"Błąd odczytu: %s\n", fileName);
	}
	unsigned int meshesAmount = scene -> mNumMeshes;
	unsigned i = 0;
	if (whichMesh != nullptr )
	{
		if (*whichMesh < meshesAmount)
		{
			i = *whichMesh;
			meshesAmount = i + 1;
		}
		else fprintf(stderr,"nie ma siatki o takim numerze wczytuje 0: %s\n", fileName);
		{
			i = 0;
			meshesAmount = i + 1;
		}
	}
	for(; i < meshesAmount ; i++) //!!! wczytywanie wszytkich czesci na raz
	{
		aiMesh * mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;
		if (mesh->HasTextureCoords(0))
			hasTextureCoords = 1;
		else
			fprintf(stderr,"plik nie współrzędnych teksturowania: %s\n", fileName);
		if (!mesh->HasNormals())
			fprintf(stderr,"plik nie ma normalnych: %s\n", fileName);
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
	assignVBO("vertex", vertexbuffer, 3);
	assignVBO("Normals", normalsBuffer, 3);
	assignVBO("vertexTexture", textureBuffer, 2);
	glBindVertexArray(0);
	verticesAmount = totalVertices;
	if (whichMesh != nullptr)
		*whichMesh = scene -> mNumMeshes;
	enableLight();
}
Model::~Model()
{
	glDeleteTextures(1,&texture);
}
int Model::textureLoad(const char * fileName)
{
	if (hasTextureCoords)
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
		hasTexture = 1;
		return 0;
	}
	return 1;
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
void Model::assignVBO(const char * name, GLuint buf, int points)
{
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
