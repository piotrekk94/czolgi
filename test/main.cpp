#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
//#include <glm/gtx/ro>

#include "shader.hpp"
#include "model.hpp"
float speed_x, speed_y;
float camera = 10.0f;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLFWwindow* window; 
GLuint VertexArrayID;
	glm::mat4 mvp;
static const GLfloat triangle[] = {

	-0.3f, -0.3f, 0.0f, 1.0f,
	0.3f, -0.3f, 0.0f, 1.0f,
	0.0f,  0.3f, 0.0f, 1.0f,
};
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
int drawScene();
void timeMeasure();
int drawCube(GLuint &vertexbuffer, ShaderProgram Shader);
int initGL(int x, int y);
void Display();
int main(int argc, char ** argv)
{
	initGL(1024, 768);
	glClearColor(0,0,0.2,0);
	//	glutMainloop();
	ShaderProgram Shader("vertex.vert", "fragment.frag");
	Shader.on();
	// This will identify our vertex buffer
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
//		GLuint vertexbuffer;
//	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
//	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	std::vector<GLfloat> vertex(g_vertex_buffer_data, g_vertex_buffer_data + (3 * 12 * 4) );

	std::vector<GLfloat> trianglev(triangle, triangle + 3 * 4);
	Model cube("models/cube.obj" , &Shader, VertexArrayID);
	Model cube3("models/cube.obj" , &Shader, VertexArrayID);
	Model cube2("models/zero.obj" , &Shader, VertexArrayID);
	cube.textureLoad("./tekstura.bmp");

	float angle_x = 0, angle_y = 0;
	glm::mat4 PMatrix = glm::perspective(float(50 ), float(1024/768), 1.0f, 50.0f);
	double time = glfwGetTime();
//	double time2 = time;
	do{
		timeMeasure();
	glm::mat4 VMatrix = glm::lookAt(
			glm::vec3(camera, camera, camera),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
	angle_x += speed_x * (glfwGetTime() - time);	
	angle_y += speed_y * (glfwGetTime() - time);
	time = glfwGetTime();
	glm::mat4 ModelMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(0.5f, 0.5f, 0.1f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1.0f, -1.0f, -1.0f));
	//glm::mat4 ModelMatrix2 = glm::translate(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
	ModelMatrix = glm::rotate(ModelMatrix, angle_x, glm::vec3(1,0,0));
	ModelMatrix = glm::rotate(ModelMatrix, angle_y, glm::vec3(0,1,0));
		drawScene();
		mvp = PMatrix * VMatrix * ModelMatrix;
	//	glm::mat4 mvp2 = PMatrix * VMatrix * ModelMatrix2;
		cube.setMMatrix(glm::mat4(1.0f));// reset
		cube.scale(glm::vec3(0.5f, 0.5f, 0.1f));
		cube.translate(glm::vec3(-1,-1,5));
		cube.rotate(angle_x, glm::vec3(1,0,0));
		cube.rotate(angle_y, glm::vec3(0,1,0));
		cube.setVMatrix(VMatrix);
		cube.setPMatrix(PMatrix);
		cube.setMVPMatrix(mvp);
		cube.draw();
		cube2.setMMatrix(glm::mat4(1));
		cube2.translate(glm::vec3(0,0,0));
		cube2.setVMatrix(VMatrix);
		cube2.setPMatrix(PMatrix);
		cube2.draw();
		cube3.setMMatrix(glm::mat4(1));
		cube3.translate(glm::vec3(0,0,0));
		cube3.scale(glm::vec3(0.1,0.1,0.1));
		cube3.setVMatrix(VMatrix);
		cube3.setPMatrix(PMatrix);
		cube3.draw();

	//	drawCube(vertexbuffer, Shader);
		glfwSwapBuffers(window); //
		glfwPollEvents(); // od klawiszy
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0 );
	//Cleanup VBO
	//glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}
void timeMeasure()
{
	static int counter;
	static double time1 = glfwGetTime();
	double time2 = glfwGetTime();
	counter++;
	if (time2 > 1 + time1)
	{
		fprintf(stderr,"%f ms/frame\n",(time2 - time1) * 1000 / counter);
		time1 = time2;
		counter = 0;
	}
	return;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_LEFT) speed_y = -3.14;
		if (key == GLFW_KEY_RIGHT) speed_y = 3.14;
		if (key == GLFW_KEY_UP) speed_x = -3.14;
		if (key == GLFW_KEY_DOWN) speed_x = 3.14;
		if (key == GLFW_KEY_W) camera += 0.5f;
		if (key == GLFW_KEY_S) camera -= 0.5f;
	}
	if (action == GLFW_RELEASE) 
	{
		speed_y = 0;
		speed_x = 0;
	}
}
int drawCube(GLuint &vertexbuffer,ShaderProgram Shader)
{
	GLuint Shadermpv = Shader.getUniformLocation("mvp");
	glBindVertexArray(VertexArrayID);
	glUniformMatrix4fv(Shadermpv, 1, false, glm::value_ptr(mvp));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	return 0;
}
int drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return 0;
}

int initGL(int x, int y) //??? do poprawy
{
	glfwInit();
	//	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	// Open a window and create its OpenGL context
	window = glfwCreateWindow( x, y, "Tutorial 01", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	// aktywuj VAO
	glGenVertexArrays(1, &VertexArrayID);
	// aktywuj VAO
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(window, key_callback);
	glEnable(GL_DEPTH_TEST); // z-bufor
	return 0;
}
