#version 330 core

in VERT_OUT
{
	int sPos,data;
	mat4 camera;
} vert_in[];

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

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
	vec2(1.0f,1.0f),
	vec2(1.0f,0.0f)
);
const float light[] = float[]
(
	1.0f,
	0.7f,
	0.3f,
	0.7f,
	0.3f,
	0.0f
);

out float lVal;

void main()
{
	vec3 pos=gl_in[0].gl_Position.xyz;
	for (int i=vert_in[0].data*4;i<vert_in[0].data*4+4;i++)
	{
		gl_Position = vert_in[0].camera*vec4(pos.xyz+vertex[indices[i]],1.0f);
		lVal = light[vert_in[0].data];
		EmitVertex();
	}
	EndPrimitive();
}