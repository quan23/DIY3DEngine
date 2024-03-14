#version 330 core

layout (location = 0) in int aPos;
layout (location = 1) in int data;

out VERT_OUT
{
	int aPos,data;
} vert_out;


uniform int CHUNK_WIDTH;
uniform int CHUNK_LENGTH;
uniform int CHUNK_HEIGHT;

uniform int chunkX;
uniform int chunkY;
uniform int chunkZ;

uniform mat4 model;

uniform int time;


void main()
{
	ivec3 ver;
	int Pos = aPos;
	ver.x = Pos % CHUNK_WIDTH;
	Pos = Pos / CHUNK_WIDTH;
	ver.z = Pos % CHUNK_LENGTH;
	Pos = Pos / CHUNK_LENGTH;
	ver.y = Pos % CHUNK_HEIGHT;
	Pos = Pos / CHUNK_HEIGHT;

	gl_Position = vec4(ver + vec3(chunkX*CHUNK_WIDTH,chunkY*CHUNK_HEIGHT,chunkZ*CHUNK_LENGTH),1.0f);

	vert_out.aPos=aPos;
	vert_out.data=data;
}
