#include "model.hpp"
//zmienne statyczne
std::vector<Light> Model::light;
glm::mat4 Model::VMatrix = glm::mat4(1.0f);
glm::mat4 Model::PMatrix = glm::mat4(1.0f);
float Model::ambient = 0;
//int Model::globalTextureNumber = 0;

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
	shader->use();
	sendUniformData();
	if (hasTexture)
	{
		for (unsigned i=0; i < textures.size() ; i++)
		{
			textures[i]->activate();
			/*
			glActiveTexture(textureNumber[i]);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			*/
		}
	}
	vbo->activate();
	glDrawArrays(GL_TRIANGLES, 0, verticesAmount );
	//glDisableVertexAttribArray(0);
	//
	vbo->deactivate();
	if (hasTexture)
	{
		for (unsigned i=0; i < textures.size() ; i++)
		{
			textures[i]->deactivate();
	/*		glActiveTexture(textureNumber[i]);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			*/
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
	{
		int * locationArray = new int[textures.size()];
		for (unsigned i=0; i < textures.size() ; i++)
		{
			locationArray[i] = textures[i]->getTextureLocation();
		}
		glUniform1iv(shader->getUniformLocation("textureArray"),textures.size(), &locationArray[0]);
		delete[] locationArray;
	}
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
Model::Model(ShaderProgram *shader, std::vector<glm::vec3> *points, std::vector<glm::vec2> *textureCoordinates , std::vector<glm::vec3> *normals)
{
vbo = std::make_shared<VBO>(shader, points, textureCoordinates, normals);
verticesAmount = vbo->getVerticesAmount();
}
Model::Model(const char * fileName, ShaderProgram * shader, unsigned *whichMesh)
{
#if DEBUG == 1
	fprintf(stderr,"ladowanie modelu: %s\n", fileName);
	clock_t begin = clock();
#endif
	this->shader = shader;
	if (whichMesh != nullptr)//!!!
		*whichMesh = readOBJ(fileName, whichMesh);
	else readOBJ(fileName);
	verticesAmount = vbo->getVerticesAmount();
	//glGenVertexArrays(1,&vertexArrayID); // ??? osobne dla kazdego modelu czy nie?

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
	vbo = std::make_shared<VBO>(shader, fileName, &hasTextureCoords, whichMesh);
	return 0;
}
Model::~Model()// tu pewnie duzo brakuje
{
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
int Model::textureLoad(const char * fileName, int number)
{
	//int location = -1;
#if DEBUG == 1
	fprintf(stderr,"ladowanie tekstury: %s\n", fileName);
	clock_t begin = clock();
#endif
	if (hasTextureCoords)
	{
		if ((number < 0) || ((unsigned)number >= textures.size())) //jesli nie ma takiego numeru wczytaj jako nową
		{
			number = textures.size();
			textures.push_back(nullptr);
		}
		textures[number] = std::make_shared<Texture>();
		textures[number]->read(fileName);
		hasTexture = 1;
	}
#if DEBUG == 1
	clock_t end = clock();
	double elapsed = (1000 * double(end - begin)/CLOCKS_PER_SEC);
	fprintf(stderr,"czas: %f ms\n", elapsed);
#endif
	return textures[number]->getTextureLocation();
}
int Model::bumpTextureLoad(const char * fileName, int number)
{
	if ((number < 0) || ((unsigned)number >= textures.size())) //jesli nie ma takiego numeru wczytaj jako nową
	{
		hasBump = textures.size();
	}
	else
	{
		hasBump = number;
	}
	return textureLoad(fileName, number);
}
int Model::textureShare(const std::shared_ptr<Texture> *ptr, int number)
{
	if (hasTextureCoords)
	{
		if ((number < 0) || ((unsigned)number >= textures.size())) //jesli nie ma takiego numeru wczytaj jako nową
		{
			number = textures.size();
			textures.push_back(nullptr);
		}
		textures[number] = *ptr;
	}
	return 0;
}
int Model::bumpTextureShare(const std::shared_ptr<Texture> *ptr, int number)
{

	if ((number < 0) || ((unsigned)number >= textures.size())) //jesli nie ma takiego numeru wczytaj jako nową
	{
		hasBump = textures.size();
	}
	else
	{
		hasBump = number;
	}
	return textureShare(ptr, number);
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
