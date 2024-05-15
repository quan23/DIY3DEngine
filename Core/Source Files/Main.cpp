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

static std::pair<Chunk*, Block*> hitBlock(World& world, Camera& camera, float maxLength)
{
	int maxCal = 1000;
	glm::vec3 target = camera.getCoor(), rayDire = camera.Orientation, stepNeed = glm::vec3(0.0f);
	auto inBoundOf = [](glm::vec3 coor)
	{
		return blockCoor(int(coor.x) + ((coor.x < 0.0f) ? -1 : 0), int(coor.y) + ((coor.y < 0.0f) ? -1 : 0), int(coor.z) + ((coor.z < 0.0f) ? -1 : 0));
	};
	//if (world.getChunk(worldCoor(0, 0, 0)) == nullptr) return { nullptr,nullptr };
	blockCoor targetBlock = inBoundOf(target);
	Block* block = world.getBlock(targetBlock);
	if (block == nullptr) return { nullptr,nullptr };
	blockCoor nextBlock;
	auto checkDire = [](GLfloat rayDire, long long& nextBlock, int targetBlock, GLfloat& stepNeed, GLfloat target)
	{
		if (rayDire == 0.0f)
			nextBlock = 0;
		else if (rayDire < 0.0f)
			nextBlock = -1;
		else nextBlock = 1;
		if (nextBlock != 0) stepNeed = abs(std::max(target - (targetBlock + nextBlock + 1), (targetBlock + nextBlock) - target) / rayDire);
		else stepNeed = 9999.0f;
	};
	checkDire(rayDire.x, nextBlock.x, targetBlock.x , stepNeed.x, target.x);
	checkDire(rayDire.y, nextBlock.y, targetBlock.y , stepNeed.y, target.y);
	checkDire(rayDire.z, nextBlock.z, targetBlock.z , stepNeed.z, target.z);
	while (maxLength > 0.0f && block->blockID == 0)
	{
		if (nextBlock.x != 0 && (stepNeed.x <= stepNeed.y && stepNeed.x <= stepNeed.z))
		{
			target += rayDire * glm::vec3(stepNeed.x);
			maxLength -= stepNeed.x;
			stepNeed -= glm::vec3(stepNeed.x);
			//targetBlock = inBoundOf(target);
			targetBlock.x += nextBlock.x;
			stepNeed.x = abs(std::max(target.x - (targetBlock.x + nextBlock.x + 1), (targetBlock.x + nextBlock.x) - target.x)/rayDire.x);
		}
		else if (nextBlock.y != 0 && (stepNeed.y <= stepNeed.x && stepNeed.y <= stepNeed.z))
		{
			target += rayDire * glm::vec3(stepNeed.y);
			maxLength -= stepNeed.y;
			stepNeed -= glm::vec3(stepNeed.y);
			//targetBlock = inBoundOf(target);
			targetBlock.y += nextBlock.y;
			stepNeed.y = abs(std::max(target.y - (targetBlock.y + nextBlock.y + 1), (targetBlock.y + nextBlock.y) - target.y) / rayDire.y);
		}
		else if (nextBlock.z != 0 && (stepNeed.z <= stepNeed.y && stepNeed.z <= stepNeed.x))
		{
			target += rayDire * glm::vec3(stepNeed.z);
			maxLength -= stepNeed.z;
			stepNeed -= glm::vec3(stepNeed.z);
			//targetBlock = inBoundOf(target);
			targetBlock.z += nextBlock.z;
			stepNeed.z = abs(std::max(target.z - (targetBlock.z + nextBlock.z + 1), (targetBlock.z + nextBlock.z) - target.z) / rayDire.z);
		}
		
		//std::cout << maxLength << "\n";
		if (maxLength < 0.0f)
		{
			//std::cout << "bruh\n";
			//std::cout << target.x << " " << target.y << " " << target.z << " " << targetBlock << "out\n";

			return { nullptr,nullptr };
		};
		block = world.getBlock(targetBlock);
		if (block == nullptr)
		{
			//std::cout << target.x << " " << target.y << " " << target.z << targetBlock << "null\n";
			return { nullptr,nullptr };
		}
		if (maxCal--< 0)
		{
			//std::cout << target.x << " " << target.y << " " << target.z << targetBlock << "skull\n";
			//std::cout << "skull ";
			std::cout << rayDire.x << " " << rayDire.y << " " << rayDire.z << "skull\n";
			return { nullptr,nullptr };
		}
		
	}
	//std::cout << target.x << " " << target.y << " " << target.z << targetBlock << "hit\n";
	std::cout << targetBlock << "\n";
	if (maxLength > 0.0f) return {world.getChunk(World::inWhatChunk(targetBlock)), block };
	else return { nullptr,nullptr };
}

int main()
{
	using namespace std;
	using namespace glm;
	
	Window window(1080, 720, "Main");
	double tStart, tEnd;

	//Shader ShaderProgram("default.vert", "default.frag");
	Shader ShaderProgram("Core/Resource Files/Shaders/Voxel.vert", "Core/Resource Files/Shaders/default.frag", "Core/Resource Files/Shaders/Voxel.geom");
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
	world.updateWorldAnchor(worldCoor(0, 0, 0));
	

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

		
		//if (block.second != nullptr)
		{
			
			if (glfwGetMouseButton(window.getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				std::pair<Chunk*, Block*> block = hitBlock(world, Camera, 6.0f);
				if (block.second != nullptr)
				{
					std::cout << (int)block.second->blockID << " ";
					block.second->blockID = 0;
					block.first->ShouldUpdate = true;
				}
			}
		}

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

