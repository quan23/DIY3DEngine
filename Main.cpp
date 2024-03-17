#include <iostream>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <thread>

#include "Window.h"
#include "ShaderClass.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Cube.h"
#include "DataType.h"
#include "World.h"

void checkerror()
{
	GLenum error=glGetError();
	while (error)
	{
		std::cout << error << " ";
		error = glGetError();
	}
	std::cout << "\n";
}

int main()
{
	using namespace std;
	using namespace glm;
	int sWidth = 1780, sHeight = 1020;
	
	Window window(sWidth, sHeight, "Main");
	double tStart, tEnd;

	//Shader ShaderProgram("default.vert", "default.frag");
	Shader ShaderProgram("Voxel.vert", "default.frag", "Voxel.geom");
	//Shader ShaderProgram("Voxel.vert", "test.frag", "test.geom");
	stbi_set_flip_vertically_on_load(true);

	camera Camera(sWidth, sHeight, glm::vec3(10.f));

	
	GLuint cameraID = glGetUniformLocation(ShaderProgram.ID, "camera");

	GLuint cPosID = glGetUniformLocation(ShaderProgram.ID, "cPos");

	//Texture text0("Default/stone.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RED, GL_UNSIGNED_BYTE);
	//text0.linkTex(ShaderProgram, "tex0", 0);
	TextureAtlas atlas("Default", GL_TEXTURE0, GL_UNSIGNED_BYTE, 16);
	atlas.linkTex(ShaderProgram, "tex0", 0);

	//Texture text1("Default/grass_block_top.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RED, GL_UNSIGNED_BYTE);
	//text1.linkTex(ShaderProgram, "tex0", 0);
	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lPos"), 1, &vec3(10.0f, 10.0f, 10.0f)[0]);
	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lDir"), 1, &normalize(vec3(-0.6f,1.0f,0.2f))[0]);
	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lCol"), 1, &vec3(1.0f, 1.0f, 1.0f)[0]);

	ShaderProgram.Activate();
	glfwSetCursorPos(window.getWindow(), (double)sWidth / 2, (double)sHeight / 2);
	tStart = glfwGetTime();

	
	atlas.Bind();

	World world(ShaderProgram.ID);
	world.updataRenderDist(5);
	world.startLoading();
	world.updateWorldAnchor(worldCoor(0,0,0));

	Camera.setSpeed(10.0f);

	checkerror();
	while (!window.shouldClose() && !(glfwGetKey(window.getWindow(), GLFW_KEY_BACKSPACE) == GLFW_PRESS))
	{
		tEnd = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Camera.cMatrix(90.0f, 0.1f, 1000.0f, cameraID);

		//world.loadWorld(Camera.getWorldCoor(), 5);
		//world.reloadWorld();

		world.updateWorldAnchor(Camera.getWorldCoor());
		

		world.renderWorld();

		world.pushAllChunk();

		Camera.cInput(window.getWindow(), tEnd - tStart);
		glUniform3fv(cPosID, 1, &Camera.getCoor()[0]);

		window.update();

		tStart = tEnd;
	}
	std::cout << Chunk::totalFace;
	world.endloading();
	atlas.Delete();
	ShaderProgram.Delete();
	return 0;
}

