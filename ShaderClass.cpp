#include "ShaderClass.h"


std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char *VertexFile, const char *FragmentFile)
{
	std::string VertexCode = get_file_contents(VertexFile);
	std::string FragmentCode = get_file_contents(FragmentFile);

	const char *VertexSource = VertexCode.c_str();
	const char *FragmentSource = FragmentCode.c_str();

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexSource, NULL);
	glCompileShader(VertexShader);

	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentSource, NULL);
	glCompileShader(FragmentShader);

	ID = glCreateProgram();
	glAttachShader(ID, VertexShader);
	glAttachShader(ID, FragmentShader);
	glLinkProgram(ID);

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}
