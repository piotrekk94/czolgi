#include "model.hpp"
int Model::draw()
{

	shader->on();
	glUniformMatrix4fv(	shader->getUniformLocation("mvp"), 1, false, glm::value_ptr(MVPMatrix));
	// Draw the triangle !
	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, verticesAmount ); // Starting from vertex 0; 3 vertices total -> 1 triangle
	//glDisableVertexAttribArray(0); // to usuwa
	glBindVertexArray(0);
}
Model::Model(std::vector<GLfloat> &inVertex, ShaderProgram * shader, GLuint vertexArrayID)
{
	this->shader = shader;
	//this->vertex = inVertex;
	this->vertexArrayID = vertexArrayID;
	verticesAmount = inVertex.size() / 3;
	//
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, verticesAmount * 3 * sizeof(inVertex[0]), &inVertex[0], GL_STATIC_DRAW);
	
	//
	//glGenVertexArrays(1,&vertexArrayID); // ??? osobne dla każdego modelu czy nie?
	glBindVertexArray(this->vertexArrayID);
	assignVBO("vertex");
	glBindVertexArray(0);
}
Model::Model(const char * fileName, ShaderProgram * shader, GLuint vertexArrayID)
{
	std::vector<GLfloat> vertices;
	int totalVertices = 0;
	Assimp::Importer importer;
	//glGenVertexArrays(1,&vertexArrayID); // ??? osobne dla każdego modelu czy nie?
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
		}
		totalVertices += mesh->mNumVertices;
	}
	//
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	//
	//glGenVertexArrays(1,&vertexArrayID); // ??? osobne dla każdego modelu czy nie?
	glBindVertexArray(this->vertexArrayID);
	assignVBO("vertex");
	glBindVertexArray(0);
	verticesAmount = totalVertices;

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
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
}
