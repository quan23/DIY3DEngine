#include <iostream>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <thread>

#include "Window.h"
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
	int sWidth = 1280, sHeight = 720;
	
	Window window(sWidth, sHeight, "Main");
	double tStart, tEnd;

	Shader ShaderProgram("default.vert", "default.frag");
	stbi_set_flip_vertically_on_load(true);

	camera Camera(sWidth, sHeight, glm::vec3(10.f));
	GLuint cameraID = glGetUniformLocation(ShaderProgram.ID, "camera");
	GLuint cPosID = glGetUniformLocation(ShaderProgram.ID, "cPos");

	Texture plank("Default/stone.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RED, GL_UNSIGNED_BYTE);
	plank.linkTex(ShaderProgram, "tex0", 0);
	Texture plankSpe("grass_block_side.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGB, GL_UNSIGNED_BYTE);
	plankSpe.linkTex(ShaderProgram, "tex1", 1);

	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lPos"), 1, &vec3(10.0f, 10.0f, 10.0f)[0]);
	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lDir"), 1, &normalize(vec3(-0.6f,1.0f,0.2f))[0]);
	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lCol"), 1, &vec3(1.0f, 1.0f, 1.0f)[0]);

	ShaderProgram.Activate();
	glfwSetCursorPos(window.getWindow(), (double)sWidth / 2, (double)sHeight / 2);
	tStart = glfwGetTime();

	plank.Bind();
	plankSpe.Bind();
	mat4 rota = mat4(1.0f);
	rota = rotate(rota, radians(0.1f), vec3(0.0f, 1.0f, 0.0f));
	World world(ShaderProgram.ID);
	world.updataRenderDist(5);
	world.startLoading();
	world.updateWorldAnchor(worldCoor(0,0,0));

	Camera.setSpeed(10.0f);
	int iTime = 0;

	while (!window.shouldClose() && !(glfwGetKey(window.getWindow(), GLFW_KEY_BACKSPACE) == GLFW_PRESS))
	{
		tEnd = glfwGetTime();

		glUniform1i(glGetUniformLocation(ShaderProgram.ID, "time"), ++iTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Camera.cMatrix(90.0f, 0.1f, 1000.0f, cameraID);

		//world.loadWorld(Camera.getWorldCoor(), 5);
		//world.reloadWorld();
		world.updateWorldAnchor(Camera.getWorldCoor());
		world.pushAllChunk();
		

		world.renderWorld();

		Camera.cInput(window.getWindow(), tEnd - tStart);
		glUniform3fv(cPosID, 1, &Camera.getCoor()[0]);

		window.update();

		tStart = tEnd;
	}
	std::cout << Chunk::totalIndices;
	world.endloading();
	plank.Delete();
	plankSpe.Delete();
	ShaderProgram.Delete();
	return 0;
}

