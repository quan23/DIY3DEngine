#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <string>

std::string get_file_contents(const char* filename);

class Shader
{
	public:
		GLuint ID;
		Shader(const char *VertexFile,const char *FragmentFile);
		Shader(const char *VertexFile,const char *FragmentFile, const char *GeometryFile);

		

		void Activate();
		void Delete();
	private:
		void compileErrors(unsigned int shader, const char* type);
};
#endif
