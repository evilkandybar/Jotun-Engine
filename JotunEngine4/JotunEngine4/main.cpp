#include "stdafx.h"
#include "Shader.h"

int		main();
void	init();
void	initBackend();
void	initOpenGL();
void	initData();
void	draw();
void	onGLFWKey(GLFWwindow* window, int key, int scancode, int action, int mods);
void	onGLFWError(int error, const char *description);

GLFWwindow* window;
Shader *diffuse;

void init() {
	initBackend();
	initData();
}

void initBackend() {
	glfwSetErrorCallback(onGLFWError);
	if (!glfwInit()) {
		std::cerr << "ERROR: Could not initialize GLFW\n";
		std::cin.get();
		exit(EXIT_FAILURE);
	}
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window) {
		std::cerr << "ERROR: Could not creat window\n";
		std::cin.get();
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, onGLFWKey);

	//end of GLFW inits

	glewExperimental = GL_TRUE;
	int glewErr = glewInit();
	if( glewErr != GLEW_OK ) {
		std::cerr << "ERROR: Could not initialize GLEW\n";
		std::cin.get();
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
}

void initOpenGL() {
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(1, 0, 1, 0);

	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;
	glViewport(0, 0, width, height);
}

void initData() {
	diffuse = new Shader("DIffuse");
}

void draw() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(diffuse->getGLID());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();
	glfwSwapBuffers(window);
}

int main(void)
{
	init();

	while (!glfwWindowShouldClose(window)) {
		draw();
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void onGLFWKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void onGLFWError(int error, const char* description) {
	printf("ERROR %d: %s", error, description);
}