#include <iostream>
#include <glad/glad.h>
#include <glfw/GLFW3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "ShaderClass.h"
#include "Texture.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Cube.h"
#include "DataType.h"
#include "World.h"

int main()
{
	using namespace std;
	using namespace glm;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int sWidth = 1280, sHeight = 960;

	GLFWwindow *window = glfwCreateWindow(sWidth, sHeight, "Main", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "fail for some fucking reason \n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, sWidth, sHeight);
	
	double tStart, tEnd;

	Shader ShaderProgram("default.vert", "default.frag");
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	GLuint cameraID = glGetUniformLocation(ShaderProgram.ID, "camera");
	camera Camera(sWidth, sHeight);
	GLuint cPosID = glGetUniformLocation(ShaderProgram.ID, "cPos");

	Texture plank("planks.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	plank.linkTex(ShaderProgram, "tex0", 0);
	Texture plankSpe("planksSpec.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RED, GL_UNSIGNED_BYTE);
	plankSpe.linkTex(ShaderProgram, "tex1", 1);

	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lDir"), 1, &normalize(vec3(-0.6f,1.0f,0.2f))[0]);
	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lCol"), 1, &vec3(1.0f, 1.0f, 1.0f)[0]);

	ShaderProgram.Activate();
	glfwSetCursorPos(window, (double)sWidth / 2, (double)sHeight / 2);
	tStart = glfwGetTime();
	//vec3 lPos = vec3(-5.0f,5.0f,5.0f);
	plank.Bind();
	plankSpe.Bind();
	mat4 rota = mat4(1.0f);
	rota = rotate(rota, radians(0.1f), vec3(0.0f, 1.0f, 0.0f));
	cube Cube(vec3(1.5f),vec3(-5.0f),rota,plank,plankSpe);

	World world(ShaderProgram.ID);
	world.loadWorld(World::inWhatChunk(0,0,0), 5);

	while (!glfwWindowShouldClose(window)&&!(glfwGetKey(window,GLFW_KEY_BACKSPACE)==GLFW_PRESS))
	{
		tEnd = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Camera.cMatrix(45.0f, 0.1f, 1000.0f, cameraID);
		world.renderWorld();
		Camera.cInput(window,tEnd-tStart);
		glUniform3fv(cPosID, 1, &Camera.cPosition[0]);
		glfwSwapBuffers(window);
		glfwPollEvents();
		tStart = tEnd;
	}
	plank.Delete();
	plankSpe.Delete();
	ShaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

