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
void timeMeasure();

int windowWidth = 1024;
int windowHeight = 768;
GLFWwindow *window;
GLuint VertexArrayID;

float cameraPositionX = 4.0f;
float cameraPositionY = 3.0f;
float cameraPositionZ = 3.0f;
float FoV = 45.0; // Field of View
double xposOld; // poprzednia pozycja kursora myszy
double yposOld;

cameraDirection dir = STOP;
float cursorDX, cursorDY;


bool rotate = false;
bool lightShift = false;
float move_x, move_y;

int main(int argc, char **argv)
{
	// Initialise GLFW
	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); //  specifies the desired number of samples to use for multisampling. Zero disables multisampling. GLFW_DONT_CARE means the application has no preference.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // specifies whether the OpenGL context should be forward-compatible, i.e. one where all functionality deprecated in the requested version of OpenGL is removed. This must only be used if the requested OpenGL version is 3.0 or above.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GLFW_OPENGL_PROFILE specifies which OpenGL profile to create the context for. Possible values are one of GLFW_OPENGL_CORE_PROFILE or GLFW_OPENGL_COMPAT_PROFILE, or GLFW_OPENGL_ANY_PROFILE to not request a specific profile. If requesting an OpenGL version below 3.2, GLFW_OPENGL_ANY_PROFILE must be used.

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(windowWidth, windowHeight, "Tanks", NULL, NULL);
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

	// Keyborard tokens handling
	glfwSetKeyCallback(window, key_callback);

	// This will hide the cursor and lock it to the specified window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Cursor position handling
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwGetCursorPos(window, &xposOld, &yposOld);
	fprintf(stderr,"xposOld: %lf; yposOld: %lf\n", xposOld, yposOld);

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

	glGenVertexArrays(1, &VertexArrayID);

	mainLoop();

	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

int mainLoop()
{
	glm::mat4 mvp;
	ShaderProgram Shader("vertex.vert", "fragment.frag");

	Model cube("models/cube2.obj", &Shader);
	Model cube3("models/cube2.obj", &Shader);
	Model cube2("models/zero.obj", &Shader);
	cube.textureLoad("./tekstura.png");

	Camera cam;

	glm::vec4 lightPosition = glm::vec4(0,0,0,1);
	Light light(lightPosition);
	light.color = glm::vec3(1,1,1);

	light.power = 7;
	cube.light.push_back(light);
	light.type = 2;
	light.power = 0.3;
	light.position = glm::vec4(-1,-1,-1,0);
	Model::light.push_back(light);
	float angle_x = 0, angle_y = 0, dx = 0, dy = 0;
	float ldx = 0, ldy = 0;
	double time = glfwGetTime();

	do{
		timeMeasure();


		glm::mat4 ProjectionMatrix = glm::perspective(
	    		FoV,         // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
	    		4.0f / 3.0f, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
	    		0.1f,        // Near clipping plane. Keep as big as possible, or you'll get precision issues.
	    		100.0f       // Far clipping plane. Keep as little as possible.
		);



		cam.move(dir);
		cam.rotate(cursorDX, cursorDY);
		cursorDX = cursorDY = 0;
		glm::mat4 ViewMatrix = cam.getVMatrix();

		Model::setVMatrix(ViewMatrix);//raz dla wszystkich instacji
		Model::setPMatrix(ProjectionMatrix);//raz dla wszystkich instancji

		cube.light[0].position = glm::vec4(ldx,ldy,0,1);
		time = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.setPos(dx,dy,1);
		cube.setAngle(angle_x,angle_y,0.0f);
		cube.setCenter(0.5, 0.5, 0.5);
		cube.draw();

		cube2.draw();

		cube3.setPos(ldx,ldy,0);
		cube3.setScale(0.1,0.1,0.1);
		cube3.setCenter(0.5, 0.5, 0.5);
		cube3.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_LEFT:
			dir = LEFT;
			break;
		case GLFW_KEY_RIGHT:
			dir = RIGHT;
			break;
		case GLFW_KEY_UP:
			dir = FORWARD;
			break;
		case GLFW_KEY_DOWN:
			dir = BACKWARD;
			break;
		}
	}

	if (action == GLFW_RELEASE) {
		dir = STOP;
	}
}

static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
	cursorDX = xpos - xposOld;
	cursorDY = ypos - yposOld;
	xposOld = xpos;
	yposOld = ypos;
	fprintf(stderr,"xpos: %lf; ypos: %lf\n", xpos, ypos);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	if (yoffset == 1) {
		FoV-= 0.1f;
	} else if (yoffset == -1) {
		FoV += 0.1f;
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
