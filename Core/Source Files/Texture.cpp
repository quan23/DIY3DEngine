#include "Texture.h"

Texture::Texture(const char* Name, GLenum Type, GLenum slot, GLenum format, GLenum bytetype)
{
	int iWidth, iHeight, numColCh;
	type = Type;
	unsigned char* iData = stbi_load(Name, &iWidth, &iHeight, &numColCh, 0);
	this->slot = slot;
	glGenTextures(1, &ID);
	glActiveTexture(this->slot);
	glBindTexture(Type, ID);

	glTexParameteri(Type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(Type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(Type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(Type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(Type, 0, GL_RGBA, iWidth, iHeight, 0, format, bytetype, iData);
	glGenerateMipmap(ID);

	stbi_image_free(iData);
	glBindTexture(Type, 0);
}

void Texture::linkTex(Shader& shader, const char* texID, GLint unit)
{
	GLuint texLoca = glGetUniformLocation(shader.ID, texID);
	shader.Activate();
	glUniform1i(texLoca, unit);
}

void Texture::Bind()
{
	glActiveTexture(slot);
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
