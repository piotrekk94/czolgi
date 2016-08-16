#teraz powinien kompilować gdy zmieni się tylko plik nagłówkowy
CCPP = g++
CPPFLAGS = -g -std=c++11 -Wall -pedantic -Iinclude -I.
LFLAGS =  -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -lGLEW -lassimp -lSOIL
BUILD_DIR = ./obj
SRC = $(wildcard *.cpp)
SRC += $(wildcard src/*.cpp)
OBJ = $(SRC:%.cpp=$(BUILD_DIR)/%.o)
DEP = $(OBJ:%.o=%.d)
OUT = czolgi
$(OUT): $(BUILD_DIR)/$(OUT)

$(BUILD_DIR)/$(OUT): $(OBJ)
	mkdir -p $(@D)
	$(CCPP) $(CPPFLAGS) -o $(OUT) $(OBJ) $(LFLAGS)

-include $(DEP)
$(BUILD_DIR)/%.o : %.cpp
	mkdir -p $(@D)
	$(CCPP) -MMD -c $< -o $@ $(CPPFLAGS)

clean :
	rm -f $(OBJ) $(OUT)
