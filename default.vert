#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in int data;
//layout (location = 2) in vec3 aNor;


out float lVal;
out vec3 fPos;
out vec2 tPos;
out vec3 fNor;
out vec3 fCol;

uniform mat4 camera;
uniform mat4 model;

uniform int CHUNK_WIDTH;
uniform int CHUNK_LENGTH;
uniform int CHUNK_HEIGHT;

uniform int chunkX;
uniform int chunkY;
uniform int chunkZ;

void main()
{
	fPos = aPos + vec3(chunkX*CHUNK_WIDTH,chunkY*CHUNK_HEIGHT,chunkZ*CHUNK_LENGTH);
	gl_Position = camera * vec4(fPos, 1.0f);
	//fNor = aNor;
	//tPos = tCor;
	//fCol = (aNor + vec3(1.0f))/2.0f;
	int tCor = data%4;
	int data = data/4;
	switch(tCor)
	{
		case 0:
			tPos = vec2(0.0f,0.0f);
			break;
		case 1:
			tPos = vec2(0.0f,1.0f);
			break;
		case 2:
			tPos = vec2(1.0f,1.0f);
			break;
		case 3:
			tPos = vec2(1.0f,0.0f);
			break;
	}
	switch(data)
	{
		case 0:
			lVal = 1.0f;
			break;
		case 1: case 3:
			lVal = 0.5f;
			break;
		case 2: case 4:
			lVal = 0.7f;
			break;
		case 5:
			lVal = 0.0f;
			break;
	}
	fCol = aPos/16.0f;
}