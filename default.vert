#version 330 core
layout (location = 0) in int aPos;
layout (location = 1) in int data;
//layout (location = 2) in vec3 aNor;

const vec3 vertex[] = vec3[]
(
	vec3(0.0f,0.0f,0.0f),
	vec3(1.0f,0.0f,0.0f),
	vec3(1.0f,1.0f,0.0f),
	vec3(0.0f,1.0f,0.0f),
	vec3(0.0f,1.0f,1.0f),
	vec3(0.0f,0.0f,1.0f),
	vec3(1.0f,0.0f,1.0f),
	vec3(1.0f,1.0f,1.0f)
);

const int indices[] = int[]
(
	3,4,7,2,
	6,7,4,5,
	5,4,3,0,
	0,3,2,1,
	1,2,7,6,
	1,6,5,0
);
const vec2 texCoor[] = vec2[]
(
	vec2(0.0f,0.0f),
	vec2(0.0f,1.0f),
	vec2(1.0f,1.0f),
	vec2(1.0f,0.0f)
);

out float lVal;
out vec3 fPos;
out vec2 tPos;
out vec3 fNor;
//out int Data;

uniform mat4 camera;
uniform mat4 model;

uniform int CHUNK_WIDTH;
uniform int CHUNK_LENGTH;
uniform int CHUNK_HEIGHT;

uniform int chunkX;
uniform int chunkY;
uniform int chunkZ;

uniform int time;
void main()
{

	//fNor = aNor;
	//tPos = tCor;
	//fCol = (aNor + vec3(1.0f))/2.0f;
	//tPos = (texCoor[data%4]+vec2(0.001f*time,0.0f));
	tPos = texCoor[data%4];

	ivec3 ver;
	int Pos = aPos;
	ver.x = Pos % CHUNK_WIDTH;
	Pos = Pos / CHUNK_WIDTH;
	ver.z = Pos % CHUNK_LENGTH;
	Pos = Pos / CHUNK_LENGTH;
	ver.y = Pos % CHUNK_HEIGHT;
	Pos = Pos / CHUNK_HEIGHT;

	fPos = ver + vertex[indices[data]] + vec3(chunkX*CHUNK_WIDTH,chunkY*CHUNK_HEIGHT,chunkZ*CHUNK_LENGTH);
	gl_Position = camera * vec4(fPos, 1.0f);
	

	switch(data/4)
	{
		case 0:
			lVal = 1.0f;
			break;
		case 1: case 3:
			lVal = 0.4f;
			break;
		case 2: case 4:
			lVal = 0.7f;
			break;
		case 5:
			lVal = 0.2f;
			break;
	}
	switch(data/4)
	{
		case 0:
			fNor = vec3(0.0f,1.0f,0.0f);
			break;
		case 1:
			fNor = vec3(0.0f,0.0f,1.0f);
			break;
		case 2:
			fNor = vec3(-1.0f,0.0f,0.0f);
			break;
		case 3:
			fNor = vec3(0.0f,0.0f,-1.0f);
			break;
		case 4:
			fNor = vec3(1.0f,1.0f,0.0f);
			break;
		case 5:
			fNor = vec3(0.0f,-1.0f,0.0f);
			break;
	}
	//Data = data/4;
}