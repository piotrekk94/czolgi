#include "model.hpp"
//zmienne statyczne
std::vector<Light> Model::light;
glm::mat4 Model::VMatrix = glm::mat4(1.0f);
glm::mat4 Model::PMatrix = glm::mat4(1.0f);
float Model::ambient = 0;
int Model::globalTextureNumber = 0;

void Model::setPos(float x, float y, float z){
	pos = glm::vec3(x, y, z);
}
void Model::setAngle(float x, float y, float z){
	angle = glm::vec3(x, y, z);
}
void Model::setScale(float x, float y, float z){
	sc = glm::vec3(x, y, z);
}
void Model::setCenter(float x, float y, float z){
	center = glm::vec3(x, y, z);
}

int Model::draw()
{

	MMatrix = glm::mat4(1.0f);
	translate(center);
	translate(pos);
	rotate(angle.y, glm::vec3(0,1,0));
	rotate(angle.z, glm::vec3(0,0,1));
	rotate(angle.x, glm::vec3(1,0,0));
	translate(-center);
	scale(sc);
	shader->on();
	sendUniformData();
	if (hasTexture)
	{
		for (unsigned i=0; i < texture.size() ; i++)
		{
			glActiveTexture(textureNumber[i]);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
		}
	}
	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, verticesAmount );
	//glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	//
	if (hasTexture)
	{
		for (unsigned i=0; i < texture.size() ; i++)
		{
			glActiveTexture(textureNumber[i]);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	return 0;
}
int Model::sendUniformData()
{

	glm::mat4 ITMV; //inverse transpose MVMatrix
	ITMV = glm::inverse(glm::transpose(VMatrix * MMatrix));
	MVPMatrix = PMatrix * VMatrix * MMatrix;
	glUniformMatrix4fv(shader->getUniformLocation("M"), 1, false, glm::value_ptr(MMatrix));
	glUniformMatrix4fv(shader->getUniformLocation("V"), 1, false, glm::value_ptr(VMatrix));
	glUniformMatrix4fv(shader->getUniformLocation("P"), 1, false, glm::value_ptr(PMatrix));
	glUniformMatrix4fv(shader->getUniformLocation("ITMV"), 1, false, glm::value_ptr(ITMV));
	glUniformMatrix4fv(shader->getUniformLocation("MVP"), 1, false, glm::value_ptr(MVPMatrix));
	glUniform1i(shader->getUniformLocation("hasTexture"), hasTexture);
	if (hasTexture)
		glUniform1iv(shader->getUniformLocation("textureArray"),textureNumber.size(), &textureNumber[0]);
	glUniform1i(shader->getUniformLocation("hasBump"), hasBump);
	glUniform1f(shader->getUniformLocation("ambient"), ambient);
	glUniform1f(shader->getUniformLocation("shinniness"), shinniness);
	glUniform4f( shader->getUniformLocation("color"), color.r, color.g, color.b, color.a);
	glUniform4f( shader->getUniformLocation("specularColor"), specularColor.r, specularColor.g, specularColor.b, specularColor.a);
	//swiatlo
	glUniform1i(shader->getUniformLocation("lightNumber"), light.size());
	for (unsigned int i = 0 ; i < light.size() ; i++)
	{
		glm::vec4 lightPosition_camera = VMatrix * light[i].position;
		std::ostringstream oss;
		oss << "light[" << i;
		glUniform4f( shader->getUniformLocation(oss.str() + "].position"), lightPosition_camera.x, lightPosition_camera.y, lightPosition_camera.z, lightPosition_camera.w);
		glUniform3f( shader->getUniformLocation(oss.str() + "].color"), light[i].color.r, light[i].color.g, light[i].color.b);
		glUniform1f( shader->getUniformLocation(oss.str() + "].power"), light[i].power);
		glUniform1i( shader->getUniformLocation(oss.str() + "].type"), light[i].type);

	}

	return 0;
}
int Model::enableLight() //nieuzywane
{
	glUniformBlockBinding(shader->getID(), shader->getUniformBlockIndex("LightBlock"), 1);//???
	glGenBuffers(1, &lightBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(light[0]), &light[0],GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightBuffer);
	return 0;
}
Model::Model(const char * fileName, ShaderProgram * shader, unsigned *whichMesh)
{
#if DEBUG == 1
	fprintf(stderr,"ladowanie modelu: %s\n", fileName);
	clock_t begin = clock();
#endif
	this->shader = shader;
	glGenVertexArrays(1,&this->vertexArrayID); // ??? raczej osobne
	if (whichMesh != nullptr)
		*whichMesh = readOBJ(fileName, whichMesh);
	else readOBJ(fileName);
	//glGenVertexArrays(1,&vertexArrayID); // ??? osobne dla kazdego modelu czy nie?
	glBindVertexArray(this->vertexArrayID);
	assignVBO("vertex", vertexbuffer, 3);
	assignVBO("Normals", normalsBuffer, 3);
	assignVBO("vertexTexture", textureBuffer, 2);
	assignVBO("bitangents", bitangentsBuffer, 3);
	assignVBO("tangents", tangentsBuffer, 3);
	glBindVertexArray(0);

	pos = glm::vec3(0, 0, 0);
	angle = glm::vec3(0, 0, 0);
	sc = glm::vec3(1, 1, 1);
	center = glm::vec3(0, 0, 0);
#if DEBUG == 1
	clock_t end = clock();
	double elapsed = (1000 * double(end - begin)/CLOCKS_PER_SEC);
	fprintf(stderr,"czas: %f ms\n", elapsed);
#endif

	//enableLight();
}
int Model::readOBJ(const char *fileName, unsigned *whichMesh)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<glm::vec2> texture;
	std::vector<glm::vec3> normals;
	int totalVertices = 0;
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile( fileName, aiProcess_Triangulate/* | aiProcess_GenNormals | aiProcess_FixInfacingNormals*/);
	if (!scene)
	{
		fprintf(stderr,"Blad odczytu: %s\n", fileName);
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
		else
		{
			fprintf(stderr,"nie ma siatki o takim numerze wczytuje 0: %s\n", fileName);
			i = 0;
			meshesAmount = i + 1;
		}
	}
	for(; i < meshesAmount ; i++)
	{
		aiMesh * mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;
		if (mesh->HasTextureCoords(0))
			hasTextureCoords = 1;
		else
			fprintf(stderr,"plik nie wspolrzednych teksturowania: %s\n", fileName);
		if (!mesh->HasNormals())
			fprintf(stderr,"plik nie ma normalnych: %s\n", fileName);
		for (int j = 0 ; j < iMeshFaces ; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			for(int k = 0 ; k < 3 ; k++)
			{
				aiVector3t<float> pos = mesh->mVertices[face.mIndices[k]];
				vertices.push_back(glm::vec3(pos.x,pos.y,pos.z));
			}
			if (mesh->HasTextureCoords(0))
			{
				for(int k = 0 ; k < 3 ; k++)
				{
					aiVector3t<float> uv = mesh->mTextureCoords[0][face.mIndices[k]];
					texture.push_back(glm::vec2(uv.x,1 - uv.y));//odwrocic y
				}
			}
			if (mesh->HasNormals())
			{
				for(int k = 0 ; k < 3 ; k++)
				{
					aiVector3t<float> temp = mesh->mNormals[face.mIndices[k]];
					normals.push_back(glm::vec3(temp.x,temp.y,temp.z));
				}
			}
			if (mesh->HasNormals() && mesh->HasTextureCoords(0))
			{
				// obliczanie tangent i bitangent
				unsigned c = vertices.size();
				// v1 - v0
				glm::vec3 deltaPos1 = vertices[c - 2] - vertices[c - 3];
				// v2 - v0
				glm::vec3 deltaPos2 = vertices[c - 1] - vertices[c - 3];
				// uv1 - uv0
				glm::vec2 deltaUV1 = texture[c - 2] - texture[c - 3];
				// uv2 - uv0
				glm::vec2 deltaUV2 = texture[c - 1] - texture[c - 3];
				//
				float denominator = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*denominator;
				glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*denominator;
				//robienie by tangent byl prostopadly ???
				glm::vec3 tangentTemp;
				glm::vec3 n = normals[c - 3];
				tangentTemp = glm::normalize(tangent - n * glm::dot(n, tangent));
				tangents.push_back(tangentTemp);
				n = normals[c - 2];
				tangentTemp = glm::normalize(tangent - n * glm::dot(n, tangent));
				tangents.push_back(tangentTemp);
				n = normals[c - 1];
				tangentTemp = glm::normalize(tangent - n * glm::dot(n, tangent));
				tangents.push_back(tangentTemp);
				//ladujemy do vectora
				bitangents.push_back(bitangent);
				bitangents.push_back(bitangent);
				bitangents.push_back(bitangent);

			}
		}
		totalVertices += mesh->mNumVertices;
	}
	// generowanie buforow i wpisanie tam wczytanych danych
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, texture.size() * sizeof(texture[0]), &texture[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tangentsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangentsBuffer);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(tangents[0]), &tangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &bitangentsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentsBuffer);
	glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(bitangents[0]), &bitangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
	//

	verticesAmount = totalVertices;
	return scene->mNumMeshes;
}
Model::~Model()// tu pewnie duzo brakuje
{
	for(unsigned i = 0; i < texture.size() ; i++)
	{
		glActiveTexture(GL_TEXTURE0 + textureNumber[i]);
		glDeleteTextures(1,&texture[i]);
	}
}
/*
int Model::textureUse(unsigned location, int number)
{
	if (location < globalTextureNumber)
	{
		return 1;
	}
	if ((number < 0) || (number >= texture.size())) //jeśli nie ma takiego numeru wczytaj jako nową
	{
		number = texture.size();
		textureNumber.push_back(location);
		texture.push_back(0);
	}
}
*/
int Model::textureLoad(const char * fileName, int mipmap, int number)
{
	int location = -1;
#if DEBUG == 1
	fprintf(stderr,"ladowanie tekstury: %s\n", fileName);
	clock_t begin = clock();
#endif

	if ((number < 0) || ((unsigned)number >= texture.size())) //jesli nie ma takiego numeru wczytaj jako nową
	{
		number = texture.size();
		textureNumber.push_back(globalTextureNumber);
		texture.push_back(0);
		globalTextureNumber++;
	}
	if (hasTextureCoords)
	{
		int width, height, channels;
		std::string fileNameStr(fileName);
		size_t dot = fileNameStr.find_last_of(".");
		std::string ext;
		if (dot != std::string::npos)
		{
			ext = fileNameStr.substr(dot);
		}
		if (ext.compare(".dds") == 0)
		{

			fprintf(stderr,"ladowanie %s\n",ext.c_str());
			unsigned char * image = SOIL_load_image(fileName, &width, &height, &channels, SOIL_FLAG_DDS_LOAD_DIRECT);
			glActiveTexture(GL_TEXTURE0 + textureNumber[number]);
			glGenTextures(1, &(texture[number]));
			glBindTexture(GL_TEXTURE_2D, texture[number]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}
		else
		{
			unsigned char * image = SOIL_load_image(fileName, &width, &height, &channels,  SOIL_LOAD_RGB);
			glActiveTexture(GL_TEXTURE0 + textureNumber[number]);
			glGenTextures(1, &(texture[number]));
			glBindTexture(GL_TEXTURE_2D, texture[number]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}
		//	if (mipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			GLfloat largestAnisotropy;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largestAnisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largestAnisotropy);
		}
		/*	else
			{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
			*/	hasTexture = 1;
		location = textureNumber[number];
	}
#if DEBUG == 1
	clock_t end = clock();
	double elapsed = (1000 * double(end - begin)/CLOCKS_PER_SEC);
	fprintf(stderr,"czas: %f ms\n", elapsed);
#endif
	return location;
}
int Model::bumpTextureLoad(const char * fileName, int number)
{
	if ((number < 0) || ((unsigned)number >= texture.size())) //jesli nie ma takiego numeru wczytaj jako nową
	{
		hasBump = texture.size();
	}
	else
	{
		hasBump = number;
	}
	return textureLoad(fileName);
}
int Model::setMMatrix(glm::mat4 MMatrix)
{
	this->MMatrix = MMatrix;
	return 0;
}
int Model::setVMatrix(glm::mat4 VMatrix)
{
	Model::VMatrix = VMatrix;
	return 0;
}
int Model::setPMatrix(glm::mat4 PMatrix)
{
	Model::PMatrix = PMatrix;
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
			location,
			points,		//rozmiar
			GL_FLOAT,       //typ
			GL_FALSE,       //znormalizowane?
			0,              // stride
			(void*)0        // array buffer offset
			);
}
