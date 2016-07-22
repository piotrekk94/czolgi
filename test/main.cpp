#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "model.hpp"

bool turn = false;
bool lightShift = false;
float speed_x, speed_y;
float cameraZoom = 10.0f;
GLFWwindow *window;
GLuint VertexArrayID;
glm::mat4 mvp;

static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f,// 1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,// 1.0f,
	-1.0f, 1.0f, 1.0f,// 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f,// 1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,// 1.0f,
	-1.0f, 1.0f,-1.0f,// 1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,// 1.0f,
	-1.0f,-1.0f,-1.0f,// 1.0f,
	1.0f,-1.0f,-1.0f,// 1.0f,
	1.0f, 1.0f,-1.0f,// 1.0f,
	1.0f,-1.0f,-1.0f,// 1.0f,
	-1.0f,-1.0f,-1.0f,// 1.0f,
	-1.0f,-1.0f,-1.0f,// 1.0f,
	-1.0f, 1.0f, 1.0f,// 1.0f,
	-1.0f, 1.0f,-1.0f,// 1.0f,
	1.0f,-1.0f, 1.0f,// 1.0f,
	-1.0f,-1.0f, 1.0f,// 1.0f,
	-1.0f,-1.0f,-1.0f,// 1.0f,
	-1.0f, 1.0f, 1.0f,// 1.0f,
	-1.0f,-1.0f, 1.0f,// 1.0f,
	1.0f,-1.0f, 1.0f,// 1.0f,
	1.0f, 1.0f, 1.0f,// 1.0f,
	1.0f,-1.0f,-1.0f,// 1.0f,
	1.0f, 1.0f,-1.0f,// 1.0f,
	1.0f,-1.0f,-1.0f,// 1.0f,
	1.0f, 1.0f, 1.0f,// 1.0f,
	1.0f,-1.0f, 1.0f,// 1.0f,
	1.0f, 1.0f, 1.0f,// 1.0f,
	1.0f, 1.0f,-1.0f,// 1.0f,
	-1.0f, 1.0f,-1.0f,// 1.0f,
	1.0f, 1.0f, 1.0f,// 1.0f,
	-1.0f, 1.0f,-1.0f,// 1.0f,
	-1.0f, 1.0f, 1.0f,// 1.0f,
	1.0f, 1.0f, 1.0f,// 1.0f,
	-1.0f, 1.0f, 1.0f,// 1.0f,
	1.0f,-1.0f, 1.0f,// 1.0f,
};

int initGL(int x, int y);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void cursor_enter_callback(GLFWwindow *window, int entered);
static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void timeMeasure();

int main(int argc, char **argv)
{
	initGL(1024, 768);

	glGenVertexArrays(1, &VertexArrayID);

	ShaderProgram Shader("vertex.vert", "fragment.frag");
	Shader.on();

	std::vector<GLfloat> vertex(g_vertex_buffer_data, g_vertex_buffer_data + (3 * 12 * 4));

	Model cube("models/cube2.obj", &Shader, VertexArrayID);
	Model cube3("models/cube2.obj", &Shader, VertexArrayID);
	Model cube2("models/zero.obj", &Shader, VertexArrayID);
	cube.textureLoad("./tekstura.png");

	glm::vec4 lightPosition = glm::vec4(0,0,0,1);
	Light light;
	light.position = (lightPosition);
	light.color = glm::vec3(1,1,1);
	light.power = 200;
	Light moonLight;
	moonLight.position = (glm::vec4(-1,-1,-1,0));
	moonLight.power = 0.4;
	moonLight.type = 2;
	cube.light.push_back(light);
	cube.light.push_back(moonLight);
	fprintf(stderr,"ilosc swiatel: %lu wielkosc struktury: %lu\n",cube.light.size(),sizeof(cube.light[0]));
	float angle_x = 0, angle_y = 0, dx = 0, dy = 0;
	float ldx = 0, ldy = 0;
	glm::mat4 ProjectionMatrix = glm::perspective(50.0f, float(1024/768), 1.0f, 50.0f);
	double time = glfwGetTime();

	do{
		timeMeasure();
		glm::mat4 ViewMatrix = glm::lookAt(
				glm::vec3(cameraZoom, cameraZoom, cameraZoom),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
		if (lightShift) {
			ldy += speed_x * (glfwGetTime() - time);
			ldx -= speed_y * (glfwGetTime() - time);
		}
		else {
			if (turn) {
				angle_x += speed_x * (glfwGetTime() - time);
				angle_y += speed_y * (glfwGetTime() - time);
			} else {
				dy += speed_x * (glfwGetTime() - time);
				dx -= speed_y * (glfwGetTime() - time);
			}
		}

		cube.light[0].position = glm::vec4(ldx,ldy,0,1);
		time = glfwGetTime();
		glm::mat4 ModelMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(0.5f, 0.5f, 0.1f));
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1.0f, -1.0f, -1.0f));
		ModelMatrix = glm::rotate(ModelMatrix, angle_x, glm::vec3(1,0,0));
		ModelMatrix = glm::rotate(ModelMatrix, angle_y, glm::vec3(0,1,0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
		cube.setMMatrix(glm::mat4(1.0f));// reset
		//cube.scale(glm::vec3(0.5f, 0.5f, 0.1f));
		cube.translate(glm::vec3(dx,dy,1));
		cube.rotate(angle_x, glm::vec3(1,0,0));
		cube.rotate(angle_y, glm::vec3(0,1,0));
		cube.translate(glm::vec3(-0.5,-0.5,-0.5));

		cube.setVMatrix(ViewMatrix);
		cube.setPMatrix(ProjectionMatrix);
		cube.setMVPMatrix(mvp);
		cube.draw();
		cube2.setMMatrix(glm::mat4(1));
		cube2.translate(glm::vec3(0,0,0));
		cube2.setVMatrix(ViewMatrix);
		cube2.setPMatrix(ProjectionMatrix);
		cube2.draw();
		cube3.setMMatrix(glm::mat4(1));
		cube.translate(glm::vec3(-0.5,-0.5,-0.5));
		cube3.translate(glm::vec3(ldx,ldy,0));
		cube3.scale(glm::vec3(0.1,0.1,0.1));
		cube3.setVMatrix(ViewMatrix);
		cube3.setPMatrix(ProjectionMatrix);
		cube3.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0);

	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

int initGL(int x, int y)
{
	// Initialise GLFW
	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 2); //  specifies the desired number of samples to use for multisampling. Zero disables multisampling. GLFW_DONT_CARE means the application has no preference.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // specifies whether the OpenGL context should be forward-compatible, i.e. one where all functionality deprecated in the requested version of OpenGL is removed. This must only be used if the requested OpenGL version is 3.0 or above.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GLFW_OPENGL_PROFILE specifies which OpenGL profile to create the context for. Possible values are one of GLFW_OPENGL_CORE_PROFILE or GLFW_OPENGL_COMPAT_PROFILE, or GLFW_OPENGL_ANY_PROFILE to not request a specific profile. If requesting an OpenGL version below 3.2, GLFW_OPENGL_ANY_PROFILE must be used.

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(x, y, "Tanks", NULL, NULL);
	if (window == NULL) {
		fprintf( stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed in openGL core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//  Makes the cursor invisible when it is over the client area of the window but does not restrict the cursor from leaving.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Keyborard tokens handling
	glfwSetKeyCallback(window, key_callback);

	// Cursor enter/leave events and its position handling
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	// Mouse wheel handling
	glfwSetScrollCallback(window, scroll_callback);

	// Background color
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	// z-bufor
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_LEFT_CONTROL:
				turn = true;
				break;
			case GLFW_KEY_LEFT_ALT:
				lightShift = true;
				break;
			case GLFW_KEY_LEFT:
				speed_y = -3.14;
				break;
			case GLFW_KEY_RIGHT:
				speed_y = 3.14;
				break;
			case GLFW_KEY_UP:
				speed_x = -3.14;
				break;
			case GLFW_KEY_DOWN:
				speed_x = 3.14;
				break;
		}
	}

	if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_LEFT_CONTROL:
				turn = false;
				break;
			case GLFW_KEY_LEFT_ALT:
				lightShift = false;
				break;
		}
		speed_y = 0;
		speed_x = 0;
	}
}

void cursor_enter_callback(GLFWwindow *window, int entered)
{
	if (entered) {
		// The cursor entered the client area of the window
	} else {
		// The cursor left the client area of the window
	}
}

static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
	fprintf(stderr,"xpos: %lf; ypos: %lf\n", xpos, ypos);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	if (yoffset == 1) {
		cameraZoom -= 0.5f;
	} else if (yoffset == -1) {
		cameraZoom += 0.5f;
	}
}

void timeMeasure()
{
	static int counter;
	static double time1 = glfwGetTime();
	double time2 = glfwGetTime();
	counter++;
	if (time2 > 1 + time1) {
		fprintf(stderr,"%f ms/frame\n", (time2 - time1) * 1000 / counter);
		time1 = time2;
		counter = 0;
	}
}
