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

Shader::Shader(const char* VertexFile, const char* FragmentFile, const char* GeometryFile)
{
	std::string VertexCode = get_file_contents(VertexFile);
	std::string FragmentCode = get_file_contents(FragmentFile);
	std::string GeometryCode = get_file_contents(GeometryFile);

	const char* VertexSource = VertexCode.c_str();
	const char* FragmentSource = FragmentCode.c_str();
	const char* GeometrySource = GeometryCode.c_str();

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexSource, NULL);
	glCompileShader(VertexShader);
	compileErrors(VertexShader, "VERTEX");

	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentSource, NULL);
	glCompileShader(FragmentShader);
	compileErrors(FragmentShader, "FRAGMENT");
	
	GLuint GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(GeometryShader, 1, &GeometrySource, NULL);
	glCompileShader(GeometryShader);
	compileErrors(GeometryShader, "GEOMETRY");

	ID = glCreateProgram();
	glAttachShader(ID, VertexShader);
	glAttachShader(ID, FragmentShader);
	glAttachShader(ID, GeometryShader);
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	glDeleteShader(GeometryShader);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}
