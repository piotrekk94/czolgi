#include "vbo.hpp"
VBO::VBO(ShaderProgram *shader, std::vector<glm::vec3> *points, std::vector<glm::vec2> *textureCoordinates , std::vector<glm::vec3> *normals, std::vector<GLuint> *indices)
{
	this->shader = shader;
	glGenVertexArrays(1,&vertexArrayID); // ??? raczej osobne
	activate();
	fprintf(stderr,"generuje buffor wierzcholkow");
	genBuf(&vertexBuffer, points);
	fprintf(stderr,"generuje buffor textur");
	genBuf(&textureBuffer, textureCoordinates );
	fprintf(stderr,"generuje buffor normalnych");
	genBuf(&normalsBuffer, normals);
	if (indices != nullptr)
	{
	fprintf(stderr,"generuje buffor ");
		elementArray = 1;
		genBuf(&elementBuffer, indices, GL_ELEMENT_ARRAY_BUFFER);
	}
	assignVBO("vertex", vertexBuffer, 3);
	assignVBO("Normals", normalsBuffer, 3);
	assignVBO("vertexTexture", textureBuffer, 2);
	verticesAmount = points->size();
	deactivate();
}
int VBO::readOBJ(const char *fileName, int *hasTextureCoords, unsigned *whichMesh)
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
			*hasTextureCoords = 1;
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
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
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

	glBindVertexArray(this->vertexArrayID);
	assignVBO("vertex", vertexBuffer, 3);
	assignVBO("Normals", normalsBuffer, 3);
	assignVBO("vertexTexture", textureBuffer, 2);
	assignVBO("bitangents", bitangentsBuffer, 3);
	assignVBO("tangents", tangentsBuffer, 3);
	glBindVertexArray(0);
	verticesAmount = totalVertices;
	return scene->mNumMeshes;

}
VBO::VBO(ShaderProgram *shader, const char *fileName, int *hasTextureCoords, unsigned *whichMesh)
{
	this->shader = shader;
	glGenVertexArrays(1,&vertexArrayID); // ??? raczej osobne
	activate();
	readOBJ(fileName, hasTextureCoords, whichMesh);
	deactivate();
}
//VBO::~VBO()
//{
	
//}
void VBO::assignVBO(const char * name, GLuint buffer, int points, GLuint type)
{
	GLuint location = shader->getAttribLocation(name);
	glBindBuffer(type, buffer);
	glEnableVertexAttribArray(location);
	//glBindBuffer(type, vertexBuffer);
	glVertexAttribPointer(
			location,
			points,		//rozmiar
			GL_FLOAT,       //typ
			GL_FALSE,       //znormalizowane?
			0,              // stride
			(void*)0        // array buffer offset
			);
}
