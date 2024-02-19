#include "Cube.h"

using namespace glm;

cube::Vertex cube::defaultVertex[] =
{
	/*glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f),
	glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f, 0.0f,-1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f, 0.0f,-1.0f),
	glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f, 0.0f,-1.0f),
								
	glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f, 0.0f, 1.0f),
								
	glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3( 1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3( 1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3( 1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3( 1.0f, 0.0f, 0.0f),
								
	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
								
	glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f, 1.0f, 0.0f),
								
	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,-1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f,-1.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,-1.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,-1.0f, 0.0f)*/
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
};

GLushort cube::defaultIndices[] =
{
	 /*0, 1, 2,
	 0, 2, 3,
	 4, 5, 6,
	 4, 6, 7,
	 8, 9, 10,
	 8, 10,11,
	 12,13,14,
	 12,14,15,
	 16,17,18,
	 16,18,19,
	 20,21,22,
	 20,22,23,*/
	0,1,2,
	0,2,3,
	4,2,1,
	4,1,4,
	5,1,0,
	5,0,6,
	6,0,3,
	6,3,7,
	7,3,2,
	7,2,4,
	7,4,5,
	7,5,6,
};

cube::cube(vec3 size, vec3 position, mat4 rotation, Texture& mainTex, Texture& specTex)
{
	for (int i = 0; i < 24; i++)
	{
		vertex[i].position = vec3(rotation * vec4(defaultVertex[i].position * size, 1.0f)) + position;
		//vertex[i].textureCoor = defaultVertex[i].textureCoor;
		//vertex[i].vertexNormal = mat3(transpose(inverse(mat3(rotation)))) * defaultVertex[i].vertexNormal;
	}
	this->mainTex = &mainTex;
	this->specTex = &specTex;
	worldP = position;
	worldR = rotation;
	worldZ = size;
	_VAO = new VAO;
	_VBO = new VBO(&vertex[0].position[0], sizeof(vertex));
	_EBO = new EBO(defaultIndices, sizeof(defaultIndices));
	_VAO->LinkVBO(*_VBO, 0, 3, 3, 0);
	//_VAO->LinkVBO(*_VBO, 1, 2, 8, 3);
	//_VAO->LinkVBO(*_VBO, 2, 3, 8, 5);
	_VAO->Unbind();
	_VBO->Unbind();
	_EBO->Unbind();
}

void cube::rotate(mat4 nRotation)
{
	worldR *= nRotation;
	for (int i = 0; i < 24; i++)
	{
		vertex[i].position = vec3(nRotation * vec4(vertex[i].position - worldP - worldZ / 2.0f, 1.0f)) + worldP + worldZ / 2.0f;
		//vertex[i].vertexNormal = mat3(transpose(inverse(mat3(nRotation)))) * vertex[i].vertexNormal;
	}
	_VBO->Delete();
	_VBO = new VBO(vertex[0].data, sizeof(vertex));
	_EBO->Bind();
	_VAO->LinkVBO(*_VBO, 0, 3, 8, 0);
	_VAO->LinkVBO(*_VBO, 2, 3, 8, 5);
	_VAO->Unbind();
	_VBO->Unbind();
	_EBO->Unbind();
}

void cube::setRotation(mat4 nRotation)
{
	worldR = nRotation;
	for (int i = 0; i < 24; i++)
	{
		vertex[i].position = vec3(worldR * vec4(defaultVertex[i].position * worldZ - worldP - worldZ / 2.0f, 1.0f)) + worldP + worldZ / 2.0f;
		//vertex[i].vertexNormal = mat3(transpose(inverse(mat3(worldR)))) * defaultVertex[i].vertexNormal;
	}
	_VBO->Delete();
	_VBO = new VBO(vertex[0].data, sizeof(vertex));
	_EBO->Bind();
	_VAO->LinkVBO(*_VBO, 0, 3, 8, 0);
	_VAO->LinkVBO(*_VBO, 2, 3, 8, 5);
	_VAO->Unbind();
	_VBO->Unbind();
	_EBO->Unbind();
}

void cube::move(vec3 nPosition)
{
	worldP += nPosition;
	for (int i = 0; i < 24; i++)
	{
		vertex[i].position = vec3(vec4(vertex[i].position, 1.0f)) + nPosition;
	}
	_VBO->Delete();
	_VBO = new VBO(vertex[0].data, sizeof(vertex));
	_EBO->Bind();
	_VAO->LinkVBO(*_VBO, 0, 3, 8, 0);
	_VAO->LinkVBO(*_VBO, 2, 3, 8, 5);
	_VAO->Unbind();
	_VBO->Unbind();
	_EBO->Unbind();
}

void cube::setPosition(vec3 nPosition)
{
	for (int i = 0; i < 24; i++)
	{
		vertex[i].position = vec3(vec4(vertex[i].position, 1.0f)) + nPosition - worldP;
	}
	worldP = nPosition;
	_VBO->Delete();
	_VBO = new VBO(vertex[0].data, sizeof(vertex));
	_EBO->Bind();
	_VAO->LinkVBO(*_VBO, 0, 3, 8, 0);
	_VAO->LinkVBO(*_VBO, 2, 3, 8, 5);
	_VAO->Unbind();
	_VBO->Unbind();
	_EBO->Unbind();
}

void cube::render()
{
	_VAO->Bind();
	mainTex->Bind();
	specTex->Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}
