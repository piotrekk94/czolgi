#pragma once

#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <ctime>
#include <iostream>
#define DEBUG 1
class Texture{
	public:
		Texture(const char * fileName);
		Texture(){};
		int activate();
		int deactivate();
		int getTexture();
		int read(const char * fileName);
		int getTextureLocation(){
			return textureLocation;};
	private:
//	GLuint textureBuffer;
	int textureLocation = -1;
	static int globalTextureNumber;
	GLuint texture;
};
