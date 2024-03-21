#include "TextureAtlas.h"
#include "DataType.h"
#include <filesystem>

namespace fs = std::filesystem;

TextureAtlas::TextureAtlas(const char* Path, GLenum slot, GLenum bytetype,const int textSize)
{
	int Width = textSize * atlasSize, Height = textSize * atlasSize, textureID = 0;
	pixel* Data = new pixel[Width * Height];
	for (const auto& texture : fs::directory_iterator(Path))
	{
		std::cout << texture.path() << " ";
		int width, height, numColCh;
		unsigned char* data = stbi_load(texture.path().string().c_str(), &width, &height, &numColCh, 0);
		std::cout << numColCh << "\n";
		for (int x = 0; x < textSize; x++)
		{
			for (int y = 0; y < textSize; y++)
			{
				pixel* Pixel = &Data[x + (textureID % atlasSize) * textSize + (y + (textureID / atlasSize) * textSize) * Width];
				switch (numColCh)
				{
				case 1:
					Pixel->r = data[(x + y * textSize) * 1 + 0];
					Pixel->g = data[(x + y * textSize) * 1 + 0];
					Pixel->b = data[(x + y * textSize) * 1 + 0];
					Pixel->a = 255;
					break;
				case 2:
					Pixel->r = data[(x + y * textSize) * 2 + 0];
					Pixel->g = data[(x + y * textSize) * 2 + 1];
					Pixel->b = 0;
					Pixel->a = 255;
					break;
				case 3:
					Pixel->r = data[(x + y * textSize) * 3 + 0];
					Pixel->g = data[(x + y * textSize) * 3 + 1];
					Pixel->b = data[(x + y * textSize) * 3 + 2];
					Pixel->a = 255;
					break;
				case 4:
					Pixel->r = data[(x + y * textSize) * 4 + 0];
					Pixel->g = data[(x + y * textSize) * 4 + 1];
					Pixel->b = data[(x + y * textSize) * 4 + 2];
					Pixel->a = data[(x + y * textSize) * 4 + 3];
					break;
				default:
					break;
				}
			}
		}
		stbi_image_free(data);
		textureID++;
	}
	this->slot = slot;
	glGenTextures(1, &ID);
	glActiveTexture(this->slot);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, bytetype, Data);
	glGenerateMipmap(ID);

	delete[] Data;
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureAtlas::linkTex(Shader& shader, const char* texID, GLint unit)
{
	GLuint texLoca = glGetUniformLocation(shader.ID, texID);
	shader.Activate();
	glUniform1i(texLoca, unit);
}

void TextureAtlas::Bind()
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void TextureAtlas::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureAtlas::Delete()
{
	glDeleteTextures(1, &ID);
}
