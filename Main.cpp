//#include<filesystem>
//namespace fs = std::filesystem;

#include <iostream>
#include <glad/glad.h>
#include <glfw/GLFW3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "ShaderClass.h"
#include "Texture.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

int main()
{
	//bala bala copy moi lan
	using namespace std;
	using namespace glm;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//vi thang glfw ngu ko biet version cua ban than nen phai cho no biet (delete van chay bth)
	//core la phien ban chi co may cai function moi
	int sWidth = 1280, sHeight = 960;

	GLFWwindow *window = glfwCreateWindow(sWidth, sHeight, "Main", NULL, NULL);
	//tao window            thay doi 2 so _________ nay de doi kich thuoc
	if (window == NULL)
	{
		std::cout << "fail for some fucking reason \n";
		glfwTerminate();
		return -1;
	}
	//thang nay khi nao fail moi thay
	glfwMakeContextCurrent(window);
	//de thang glfw biet dung cai window nao
	gladLoadGL();
	//dem thang glad ra dung
	glViewport(0, 0, sWidth, sHeight);
	//cai nay cho thang opengl biet nen dung phan nao
	//vd phan minh cho no la 500x500 tren tong 1000x800 cua window
	//copy toi day
	GLfloat Overts[] =
	{
		 0.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f,	 0.0f, 0.0f,	 0.0f, 0.0f, 1.0f,
		 0.0f, 1.0f, 0.0f,     0.0f, 1.0f, 0.0f,	 0.0f, 1.0f,	 0.0f, 0.0f, 1.0f,
		 1.0f, 1.0f, 0.0f,	   0.0f, 0.0f, 1.0f,	 1.0f, 1.0f,	 0.0f, 0.0f, 1.0f,
		 1.0f, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f,	 1.0f, 0.0f,	 0.0f, 0.0f, 1.0f,

		 0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,	 0.0f, 0.0f,	 0.0f, 0.0f, 1.0f,
		 0.0f, 1.0f, 1.0f,     1.0f, 0.0f, 1.0f,	 0.0f, 1.0f,	 0.0f, 0.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,	   1.0f, 1.0f, 0.0f,	 1.0f, 1.0f,	 0.0f, 0.0f, 1.0f,
		 1.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f,	 1.0f, 0.0f,	 0.0f, 0.0f, 1.0f,
	};
	GLushort Oindices[] =
	{
		 0, 1, 2,
		 0, 2, 3,
		 7, 6, 5,
		 7, 5, 4,
		 3, 2, 6,
		 3, 6, 7,
		 4, 5, 1,
		 4, 1, 0,
		 1, 5, 6,
		 1, 6, 2,
		 4, 0, 3,
		 4, 3, 7,
	};
	GLfloat verts[36*11] = {};
	GLushort indices[36] = {};
	for (int i = 0; i < (36); i++) indices[i] = i;
	
	for (int i = 0; i < (36) / 3; i++)
	{
		vec3 fvec = vec3(Overts[Oindices[i*3] * 11] - Overts[Oindices[i*3 + 1] * 11], Overts[Oindices[i*3] * 11 + 1] - Overts[Oindices[i*3 + 1] * 11 + 1], Overts[Oindices[i*3] * 11 + 2] - Overts[Oindices[i*3 + 1] * 11 + 2]);
		vec3 svec = vec3(Overts[Oindices[i*3 + 2] * 11] - Overts[Oindices[i*3 + 1] * 11], Overts[Oindices[i*3 + 2] * 11 + 1] - Overts[Oindices[i*3 + 1] * 11 + 1], Overts[Oindices[i*3 + 2] * 11 + 2] - Overts[Oindices[i*3 + 1] * 11 + 2]);
		vec3 normal = glm::normalize(glm::cross(svec,fvec));
		
		for (int t = 0; t < 3; t++)
		{
			for (int j = (i*3 + t) * 11; j < (i*3 + t) * 11 + 8; j++)
			{
				verts[j] = Overts[(Oindices[i*3+t]-i*3-t)*11+j];
			}
			//verts[(i*3 + t) * 11 + 6] = normal.x;
			//verts[(i*3 + t) * 11 + 7] = normal.x;
			verts[(i*3 + t) * 11 + 8] = normal.x;
			verts[(i*3 + t) * 11 + 9] = normal.y;
			verts[(i*3 + t) * 11 + 10] = normal.z;
		}
	}
	for (int i = 0; i < 36; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			cout << verts[i * 11 + j] << " ";
		}
		cout << "\n";
	}
	cout << -1;
	double tStart, tEnd;

	Shader ShaderProgram("default.vert", "default.frag");
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.4f, 0.5f, 0.5f, 1.0f);

	VAO _VAO;
	VBO _VBO(verts, sizeof(verts));
	EBO _EBO(indices, sizeof(indices));
	//VAO la pointer array chua diem
	//VBO la pointer buffer chua diem
	//EBO la pointer buffer chua id diem

	_VAO.LinkVBO(_VBO, 0, 3, 11, 0);
	_VAO.LinkVBO(_VBO, 1, 3, 11, 3);
	_VAO.LinkVBO(_VBO, 2, 2, 11, 6);
	_VAO.LinkVBO(_VBO, 3, 3, 11, 8);
	//_VAO.LinkVBO(_VBO, 1, 3);

	_VAO.Unbind();
	_VBO.Unbind();
	_EBO.Unbind();

	GLuint cameraID = glGetUniformLocation(ShaderProgram.ID, "camera");
	camera Camera(sWidth, sHeight, vec3(0.0f, 0.0f, 2.0f));
	GLuint cPosID = glGetUniformLocation(ShaderProgram.ID, "cPos");

	Texture thang_ngu("HD.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	thang_ngu.linkTex(ShaderProgram, "tex0");
	Texture cheese("cheese.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	cheese.linkTex(ShaderProgram, "tex0");

	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lDir"), 1, &normalize(vec3(-1.0f))[0]);
	glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lCol"), 1, &vec3(1.0f, 1.0f, 1.0f)[0]);

	GLint hasCompiled;
	char infoLog[1024];
	glGetProgramiv(ShaderProgram.ID, GL_LINK_STATUS, &hasCompiled);
	if (hasCompiled == GL_FALSE)
	{
		glGetProgramInfoLog(ShaderProgram.ID, 1024, NULL, infoLog);
		std::cout << "SHADER_ERROR for:" << "\n" << infoLog << std::endl;
	}
	//thang_ngu.Bind();
	//cheese.Bind();
	ShaderProgram.Activate();
	glfwSetCursorPos(window, (double)sWidth / 2, (double)sHeight / 2);
	tStart = glfwGetTime();
	vec3 lPos = vec3(1.0f,-1.0f,1.0f);
	while (!glfwWindowShouldClose(window)&&!(glfwGetKey(window,GLFW_KEY_BACKSPACE)==GLFW_PRESS))
	{
		tEnd = glfwGetTime();
		//std::cout << 1.0f / (tEnd - tStart) << "\n";
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ShaderProgram.Activate();
		Camera.cMatrix(45.0f, 0.1f, 100.0f, cameraID);
		lPos=rotate(lPos, radians(1.0f), vec3(1.0f));
		glUniform3fv(glGetUniformLocation(ShaderProgram.ID, "lPos"), 1, &lPos[0]);
		Camera.cInput(window,tEnd-tStart);
		glUniform3fv(cPosID, 1, &Camera.cPosition[0]);
		_VAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
		tStart = tEnd;
	}
	_VAO.Delete();
	_VBO.Delete();
	_EBO.Delete();
	thang_ngu.Delete();
	cheese.Delete();
	ShaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

