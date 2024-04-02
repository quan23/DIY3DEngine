#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include <glad/glad.h>
#include <glfw/GLFW3.h>

class Window
{
public:
	Window(int Width, int Height, const char* Title);
	~Window();
	
	static Window* currentWindow;
	void update() const;
	static void windowSizeCallBack(GLFWwindow* window, int width, int height);
	inline bool shouldClose() const { return glfwWindowShouldClose(ID); }
	inline GLFWwindow* getWindow() const { return ID; }
	inline int getWidth() const { return Width; }
	inline int getHeight() const{ return Height; }
private:
	bool init();
	int Width, Height;
	GLFWwindow* ID;
	const char* Title;
};

#endif // !WINDOW_CLASS_H
