#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VERT_OUT
{
	int aPos,data;
} vert_in[];

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
	3,4,2,7,
	6,7,5,4,
	5,4,0,3,
	0,3,1,2,
	1,2,6,7,
	1,6,0,5
);
const vec2 texCoor[] = vec2[]
(
	vec2(0.0f,0.0f),
	vec2(0.0f,1.0f),
	vec2(1.0f,0.0f),
	vec2(1.0f,1.0f)
);
const float light[] = float[]
(
	1.0f,
	0.7f,
	0.4f,
	0.7f,
	0.4f,
	0.2f
);
const vec3 normal[] = vec3[]
(
	vec3(0.0f,1.0f,0.0f),
	vec3(0.0f,0.0f,1.0f),
	vec3(-1.0f,0.0f,0.0f),
	vec3(0.0f,0.0f,-1.0f),
	vec3(1.0f,1.0f,0.0f),
	vec3(0.0f,-1.0f,0.0f)
);


out float lVal;
out vec3 fPos;
out vec2 tPos;
out vec3 fNor;

uniform mat4 camera;

void main()
{
	vec3 pos=gl_in[0].gl_Position.xyz;
	int face = vert_in[0].data%6;
	int textID = vert_in[0].data/6;
	for (int i=0;i<4;i++)
	{
		fPos = pos.xyz+vertex[indices[face*4+i]];
		gl_Position = camera*vec4(fPos,1.0f);
		tPos = (vec2(textID%64,textID/64)+texCoor[i])/64.0f;
		//tPos = texCoor[i];
		lVal = light[face];
		fNor = normal[face];
		EmitVertex();
	}


	EndPrimitive();
}