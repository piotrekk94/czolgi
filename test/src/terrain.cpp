#include "terrain.hpp"

Terrain::Terrain(ShaderProgram *ptrShader, const char *fileName)
{
loadHeightMap(fileName);
calculateNormals();
calculateIndices();
shader = ptrShader;
vbo = std::make_shared<VBO>(ptrShader, &points, &textureCoordinates, &normalsToPoint, &indices);
verticesAmount = vbo->getVerticesAmount();
}
int Terrain::draw()
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
		}
	}
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(columns * rows);
	vbo->activate();
	glDrawElements(GL_TRIANGLE_STRIP,
			indices.size(),
			GL_UNSIGNED_INT,
			0);//(&indices[0]));
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
int Terrain::loadHeightMap(const char *fileName)
{
	unsigned char * image = SOIL_load_image(fileName, &rows, &columns, &channels,  SOIL_LOAD_RGB);
	glm::vec3 point;
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			point.x = (float)j / (columns - 1) - 0.5f;
			point.z = (float)i / (rows - 1) - 0.5f;
			point.y = image[(j + i * columns) * 3] / 255.0f - 0.5f;//RGB
			points.push_back(point);
			textureCoordinates.push_back(glm::vec2( (j / 10.0f), (i / 10.0f) ));
		}

	}
	hasTextureCoords = 1;
return 0;
}
int Terrain::smoothHorizontal()
{
	std::vector<glm::vec3> smoothPoints;
	glm::vec3 two = glm::vec3(2);
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
		smoothPoints.push_back(points[j + i * columns]);
		if (j != columns - 1)
			smoothPoints.push_back((points[j + i * columns] + points[j + i * columns + 1]) / two);

		}

	}
	points = smoothPoints;
	columns += columns -1;
return 0;
}
int Terrain::smoothVertical()
{
	std::vector<glm::vec3> smoothPoints;
	glm::vec3 two = glm::vec3(2);
	for(int i = 0; i < rows; i++)
	{
			for(int j = 0; j < columns; j++)
			{
				smoothPoints.push_back(points[j + i * columns]);
			}
		if (i != rows - 1)
		{
			for(int j = 0; j < columns; j++)
				smoothPoints.push_back((points[j + i * columns] + points[j + (i + 1) * columns]) / two);
		}

	}
	points = smoothPoints;
	rows += rows -1;
	return 0;
}
int Terrain::calculateIndices()
{
	for(int i = 0; i < rows - 1; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			for(int k = 0; k < 2 ; k++)
			{
				unsigned int row = i + (1 - k);
				unsigned int index = row * columns + j;
				indices.push_back(index);
			}
		}
		indices.push_back(rows * columns);// primitive restart
	}
	return 0;
}
int Terrain::calculateNormals()
{
	glm::vec3 triangle0[3];
	glm::vec3 triangle1[3];
	glm::vec3 normal0;
	glm::vec3 normal1;
	for(int i = 0; i < rows - 1; i++)
	{
		for(int j = 0; j < columns - 1; j++)
		{
			triangle0[0] = points[j + i * columns];
			triangle0[1] = points[j + (i + 1) * columns];
			triangle0[2] = points[(j + 1) + (i + 1) * columns ];
			triangle1[0] = triangle0[2];
			triangle1[1] = points[(j + 1) + i  * columns];
			triangle1[2] = triangle0[0];
			normals.push_back(glm::normalize(glm::cross(
							triangle0[1] - triangle0[0],
							triangle0[2] - triangle0[0])));

			normals.push_back(glm::normalize(glm::cross(
							triangle1[1] - triangle1[0],
							triangle1[2] - triangle1[0])));
		}
	}
	glm::vec3 normalToPoint;
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			normalToPoint = glm::vec3(0);
			if ((i != 0) && (j != (columns - 1)))
				normalToPoint += normals[0 + 2 * j  + 2 * (i - 1) * (columns - 1)];
			if ((i != 0) && (j != 0))
				for (int k = 0; k < 2 ; k++)
					normalToPoint += normals[k + 2 * (j - 1) + 2 * (i - 1) * (columns - 1)];
			if ((i != (rows - 1)) && (j != (columns - 1)))
				for (int k = 0; k < 2 ; k++)
					normalToPoint += normals[k + 2 * j + 2 * i * (columns - 1)];
			if ((i != (rows - 1)) && (j != 0))
				normalToPoint += normals[1 + 2 * (j - 1) + 2 * i * (columns - 1)];
			normalsToPoint.push_back(glm::normalize(normalToPoint));
		}
	}
	return 0;
}
float Terrain::getHeight(float x, float z)
{
	float height = 0;
	int j = ((x / sc.x) + 0.5f) * (columns - 1);
	int i = ((z / sc.z) + 0.5f) * (rows - 1);
	//	if ((i < rows) && (j < columns))
	height = points[j + i * columns].y * sc.y;
	return height;
}
