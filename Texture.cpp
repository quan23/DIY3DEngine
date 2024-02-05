#include "Texture.h"

Texture::Texture(const char* iName, GLenum iType, GLenum slot, GLenum format, GLenum bytetype)
{
	int iWidth, iHeight, numColCh;
	type = iType;
	unsigned char* iData = stbi_load(iName, &iWidth, &iHeight, &numColCh, 0);

	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(iType, ID);

	glTexParameteri(iType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(iType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(iType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(iType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(iType, 0, GL_RGBA, iWidth, iHeight, 0, format, bytetype, iData);
	glGenerateMipmap(ID);

	stbi_image_free(iData);
	glBindTexture(iType, 0);
}

void Texture::linkTex(Shader& shader, const char* texID)
{
	GLuint tex0ID = glGetUniformLocation(shader.ID, texID);
	shader.Activate();
	glUniform1i(tex0ID, 0);
}

void Texture::Bind()
{
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
