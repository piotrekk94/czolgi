#include <texture.hpp>
int Texture::globalTextureNumber;

Texture::Texture(const char * fileName)
{
read(fileName);
}
int Texture::activate()
{
	glActiveTexture(GL_TEXTURE0 + textureLocation);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	return 0;
}
int Texture::deactivate()
{
	glActiveTexture(GL_TEXTURE0 +textureLocation);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return 0;
}
int Texture::read(const char * fileName)
{

	if (textureLocation < 0)
	{
		textureLocation = (globalTextureNumber);
		globalTextureNumber++;
	}
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
		glActiveTexture(GL_TEXTURE0 + textureLocation);
		glGenTextures(1, &(texture));
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	else
	{
		unsigned char * image = SOIL_load_image(fileName, &width, &height, &channels,  SOIL_LOAD_RGB);
		glActiveTexture(GL_TEXTURE0 + textureLocation);
		glGenTextures(1, &(texture));
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	//	if (mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		GLfloat largestAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largestAnisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largestAnisotropy);
	}
	/*	else
		{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		*/
	deactivate();
	return textureLocation;
}
