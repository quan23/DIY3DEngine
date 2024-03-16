#include <iostream>
#include "Window.h"

Window::Window(int Width, int Height, const char* Title)
{
	this->Width = Width;
	this->Height = Height;
	this->Title = Title;
	if (!init())
	{
		glfwTerminate();
	}
}

Window::~Window()
{
	glfwDestroyWindow(ID);
	glfwTerminate();
}

bool Window::init()
{
	if (!glfwInit())
	{
		std::cout << "glfwInit fail for some reason \n";
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	ID = glfwCreateWindow(Width, Height, Title, NULL, NULL);
	if (ID == NULL)
	{
		std::cout << "glfw fail to create window \n";
		return false;
	}
	glfwMakeContextCurrent(ID);
	gladLoadGL();
	glViewport(0, 0, Width, Height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	return true;
}

void Window::update() const
{
	glfwSwapBuffers(ID);
	glfwPollEvents();
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(ID);
}

GLFWwindow* Window::getWindow() const
{
	return ID;
}
