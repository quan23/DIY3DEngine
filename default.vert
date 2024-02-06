#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 tCor;
layout (location = 3) in vec3 aNor;


out vec3 fPos;
out vec2 tPos;
out vec3 fNor;
out vec3 fCol;

uniform mat4 camera;
uniform mat4 model;

void main()
{
	fPos = aPos;
	gl_Position = camera * vec4(fPos, 1.0f);
	fNor = aNor;
	tPos = tCor;
	fCol = (aNor + vec3(1.0f))/2.0f;
}