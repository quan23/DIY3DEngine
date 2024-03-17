#ifndef TEXTURE_ATLAS_CLASS_H
#define TEXTURE_ATLAS_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include "ShaderClass.h"

class TextureAtlas
{
public:
	GLuint ID;
	GLenum slot;
	GLuint atlasSize = 64;

	TextureAtlas(const char* Path, GLenum slot, GLenum bytetype, const int textSize);
	void linkTex(Shader& shader, const char* texID, GLint unit);
	void Bind();
	void Unbind();
	void Delete();
};

#endif