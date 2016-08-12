#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"

int mainLoop();
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void fpsMeter();
void handleKeys();

int windowWidth = 1024;
int windowHeight = 768;
GLFWwindow *window;
GLuint VertexArrayID;

Camera camera;

std::vector<Model> models;

bool keyState[1024]; //True - wcisniety

float FoV = 45.0;

double deltaTime = 0;

int main(int argc, char **argv)
{
	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "Tanks", NULL, NULL);
	if (window == NULL) {
		fprintf( stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glGenVertexArrays(1, &VertexArrayID);

	mainLoop();

	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
	return 0;
}

int mainLoop()
{
	ShaderProgram Shader("vertex.vert", "fragment.frag");

	models.push_back(Model("models/cube2.obj", &Shader));
	models.push_back(Model("models/cube2.obj", &Shader));
	models.push_back(Model("models/tygrysmaly.obj", &Shader));
	models[0].setPos(2,0,0);
	models[1].setPos(0,2,0);
	models[2].setPos(0,0,2);


	//Model cube("models/cube2.obj", &Shader);
	//Model cube3("models/cube2.obj", &Shader);
	//Model cube2("models/zero.obj", &Shader);
	//cube.textureLoad("./tekstura.png");

	glm::vec4 lightPosition = glm::vec4(0,0,0,1);
	Light light(lightPosition);
	light.color = glm::vec3(1,1,1);
	light.power = 7;
	Model::light.push_back(light);
	light.type = 2;
	light.power = 0.3;
	light.position = glm::vec4(-1,-1,-1,0);
	Model::light.push_back(light);
	float angle_x = 0, angle_y = 0, dx = 0, dy = 0;
	float ldx = 0, ldy = 0;
	glm::mat4 ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	Model::setPMatrix(ProjectionMatrix);
	do{
		double startTime = glfwGetTime();

		Model::setVMatrix(camera.getVMatrix());

		Model::light[0].position = glm::vec4(ldx,ldy,0,1);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*cube.setPos(dx,dy,1);
		cube.setAngle(angle_x,angle_y,0.0f);
		cube.setCenter(0.5, 0.5, 0.5);
		cube.draw();

		cube2.draw();

		cube3.setPos(ldx,ldy,0);
		cube3.setScale(0.1,0.1,0.1);
		cube3.setCenter(0.5, 0.5, 0.5);
		cube3.draw();*/

		for (int i = 0; i < models.size(); i++){
			models[i].draw();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		double endTime = glfwGetTime();
		deltaTime = endTime - startTime;
		fpsMeter();
		handleKeys();
	}
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	return 0;
}

void handleKeys(){
	if(keyState[GLFW_KEY_W] || keyState[GLFW_KEY_UP]){
		camera.move(FORWARD, deltaTime);
	}
	if(keyState[GLFW_KEY_S] || keyState[GLFW_KEY_DOWN]){
		camera.move(BACKWARD, deltaTime);
	}
	if(keyState[GLFW_KEY_A] || keyState[GLFW_KEY_LEFT]){
		camera.move(LEFT, deltaTime);
	}
	if(keyState[GLFW_KEY_D] || keyState[GLFW_KEY_RIGHT]){
		camera.move(RIGHT, deltaTime);
	}
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key >= 0 && key <= 1024){
		if (action == GLFW_PRESS){
			keyState[key] = true;
		} else if (action == GLFW_RELEASE){
			keyState[key] = false;
		}
	}
}

static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
	static double xposOld = 0, yposOld = 0;
	camera.rotate(xpos - xposOld, yposOld - ypos);
	xposOld = xpos;
	yposOld = ypos;
	fprintf(stderr,"xpos: %lf; ypos: %lf\n", xpos, ypos);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{

}

void fpsMeter(){
	static int fps = 0;
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	fps++;
	if (currentTime - lastTime >= 1.0){
		char title[10];
		sprintf(title,"%d FPS", fps);
		glfwSetWindowTitle(window, title);
		fps = 0;
		lastTime = currentTime;
	}
}
