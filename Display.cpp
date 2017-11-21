#include "Display.h"
#include "Constants.h"
#include <memory>
#include <GL\glfw3.h>

GLFWwindow* window;

void MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	Display::Get().HandleMouseButton(button, action, mods);
}

Display::Display()
{
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's Game of Life Simulation", NULL, NULL);
	SetMousePosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glPointSize(8);
	
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
}

Display& Display::Get()
{
	static Display display;
	return display;
}

void Display::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Display::Update()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Display::GetMouseInput(double& xPos, double& yPos)
{
	glfwGetCursorPos(window, &xPos, &yPos);
}

void Display::SetMousePosition(double x, double y)
{
	glfwSetCursorPos(window, x, y);
}

void Display::HandleMouseButton(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		lmbPressed = true;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		rmbPressed = true;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		lmbPressed = false;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		rmbPressed = false;
}

bool Display::CheckMouseButton(int key)
{
	if (key == GLFW_MOUSE_BUTTON_LEFT)
		return lmbPressed;
	if (key == GLFW_MOUSE_BUTTON_RIGHT)
		return rmbPressed;
}

bool Display::CheckKeyPress(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Display::CheckKeyRelease(int key) {
	return glfwGetKey(window, key) == GLFW_RELEASE;
}

bool Display::WindowShouldClose() {
	return glfwWindowShouldClose(window) == 0;
}