CCPP = g++
CPPFLAGS = -g -std=c++11 -Wall -pedantic
LFLAGS =  -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -lGLEW -lassimp -lSOIL
all:
	$(CCPP) $(CPPFLAGS) main.cpp shader.cpp model.cpp $(LFLAGS)
