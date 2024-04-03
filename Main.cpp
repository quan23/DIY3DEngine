#include <iostream>
#include <stb/stb_image.h>
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

static void checkerror()
{
	GLenum error=glGetError();
	while (error)
	{
		std::cout << error << " ";
		error = glGetError();
	}
	std::cout << "\n";
}

static float min(float x, float y)
{
	if (x > y) return y;
	else return x;
}

static std::pair<Chunk*,Block*> hitBlock(World& world, Camera& camera, float maxLength)
{
	int maxCal = 1000;
	glm::vec3 target = camera.getCoor(), rayDire = camera.Orientation;
	if (world.getChunk(worldCoor(0, 0, 0)) == nullptr) return { nullptr,nullptr };
	Block* block = (world.getChunk(worldCoor(0, 0, 0)))->getBlock(Coor((int)target.x + ((target.x < 0) ? -1 : 0), (int)target.y + ((target.y < 0) ? -1 : 0), (int)target.z + ((target.z < 0) ? -1 : 0)));
	if (block == nullptr) return { nullptr,nullptr };
	//std::cout << rayDire.x << " " << rayDire.y << " " << rayDire.z << "\n";
	while (maxLength > 0.0f && block->blockID == 0)
	{
		float Xdire = abs((target.x > 0.0f) ? ((int)target.x - target.x + ((rayDire.x > 0.0f) ? 0 : 1)) : (target.x - (int)target.x + ((rayDire.x > 0.0f) ? 0 : 1)));
		float Ydire = abs((target.y > 0.0f) ? ((int)target.y - target.y + ((rayDire.y > 0.0f) ? 0 : 1)) : (target.y - (int)target.y + ((rayDire.y > 0.0f) ? 0 : 1)));
		float Zdire = abs((target.z > 0.0f) ? ((int)target.z - target.z + ((rayDire.z > 0.0f) ? 0 : 1)) : (target.z - (int)target.z + ((rayDire.z > 0.0f) ? 0 : 1)));
		if (rayDire.x != 0.0f) Xdire /= abs(rayDire.x);
		else Xdire = 999.0f;
		if (rayDire.y != 0.0f) Ydire /= abs(rayDire.y);
		else Ydire = 999.0f;
		if (rayDire.z != 0.0f) Zdire /= abs(rayDire.z);
		else Zdire = 999.0f;
		if (Xdire < min(Ydire, Zdire))
		{
			target += rayDire * Xdire;
			maxLength -= Xdire;
		}
		else if (Ydire < min(Xdire, Zdire))
		{
			target += rayDire * Ydire;
			maxLength -= Ydire;
		}
		else
		{
			target += rayDire * Zdire;
			maxLength -= Zdire;
		}
		//std::cout << maxLength << "\n";
		if (maxLength < 0.0f)
		{
			std::cout << "bruh\n";
			return { nullptr,nullptr };
		};
		block = (world.getChunk(worldCoor(0, 0, 0)))->getBlock(Coor((int)target.x + ((target.x < 0) ? -1 : 0), (int)target.y + ((target.y < 0) ? -1 : 0), (int)target.z + ((target.z < 0) ? -1 : 0)));
		if (block == nullptr) return { nullptr,nullptr };
		if (maxCal--< 0)
		{
			std::cout << "skull\n";
			return { nullptr,nullptr };
		}
	}
	if (maxLength > 0.0f) return {world.getChunk(world.inWhatChunk(Coor((int)target.x,(int)target.y,(int)target.z))), block };
	else return { nullptr,nullptr };
}

int main()
{
	using namespace std;
	using namespace glm;
	
	Window window(1080, 720, "Main");
	double tStart, tEnd;

	//Shader ShaderProgram("default.vert", "default.frag");
	Shader ShaderProgram("Voxel.vert", "default.frag", "Voxel.geom");
	//Shader ShaderProgram("Voxel.vert", "test.frag", "test.geom");
	stbi_set_flip_vertically_on_load(true);

	Camera Camera(window, glm::vec3(10.f));

	
	GLuint cameraID = glGetUniformLocation(ShaderProgram.ID, "camera");

	GLuint cPosID = glGetUniformLocation(ShaderProgram.ID, "cPos");

	//Texture text0("Default/stone.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RED, GL_UNSIGNED_BYTE);
	//text0.linkTex(ShaderProgram, "tex0", 0);
	TextureAtlas atlas("Default", GL_TEXTURE0, GL_UNSIGNED_BYTE, 16);
	atlas.linkTex(ShaderProgram, "tex0", 0);

	//Texture text1("Default/grass_block_top.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RED, GL_UNSIGNED_BYTE);
	//text1.linkTex(ShaderProgram, "tex0", 0);
	//glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lPos"), 1, &vec3(10.0f, 10.0f, 10.0f)[0]);
	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lDir"), 1, &normalize(vec3(-0.6f,1.0f,0.2f))[0]);
	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lCol"), 1, &vec3(1.0f, 1.0f, 1.0f)[0]);

	ShaderProgram.Activate();
	tStart = glfwGetTime();

	
	atlas.Bind();

	World world(ShaderProgram.ID);
	world.updataRenderDist(4);
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

		//world.updateWorldAnchor(Camera.getWorldCoor());
		
		world.pushAllChunk();

		world.renderWorld();

		/*std::pair<Chunk*, Block*> block = hitBlock(world, Camera, 6.0f);
		if (block.second != nullptr)
		{
			
			if (glfwGetMouseButton(window.getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				std::cout << (int)block.second->blockID << " ";
				block.second->blockID = 0;
				block.first->ShouldUpdate = true;
			}
		}*/

		std::cout << window.getHeight() << " " << window.getWidth() << "\n";


		Camera.cInput(tEnd - tStart);
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

