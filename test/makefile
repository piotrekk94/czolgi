CCPP = g++
CPPFLAGS = -g -std=c++11 -Wall -pedantic -Iinclude -I.
LFLAGS =  -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -lGLEW -lassimp -lSOIL
SRC = $(wildcard *.cpp)
SRC += $(wildcard src/*.cpp)
OBJ = $(addsuffix .o, $(basename $(SRC)))
OUT = czolgi
$(OUT): $(OBJ)
	$(CCPP) $(CPPFLAGS) -o $(OUT) $(OBJ) $(LFLAGS)

%.o : %.cpp
	$(CCPP) -c $< -o $@ $(CPPFLAGS)

clean :
	rm -f $(OBJ) $(OUT)
