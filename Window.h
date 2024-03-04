#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include <glad/glad.h>
#include <glfw/GLFW3.h>

class Window
{
public:
	Window(int Width, int Height, const char* Title);
	~Window();
	
	void update() const;
	bool shouldClose() const;
	GLFWwindow *getWindow() const;
private:
	bool init();

	int Width, Height;
	GLFWwindow* ID;
	const char* Title;
};

#endif // !WINDOW_CLASS_H
