#pragma once

#include <iostream>
#include "stb_image.h"

GLuint setup_texture(const char* filename)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int w, h, chan;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pxls = stbi_load(filename, &w, &h, &chan, 0);
	if (pxls)
	{
		// Choose format based on number of channels
		GLint internalFormat;
		GLenum format;

		if (chan == 1) {
			internalFormat = GL_RED;
			format = GL_RED;
		}
		else if (chan == 3) {
			internalFormat = GL_RGB;
			format = GL_RGB;
		}
		else if (chan == 4) {
			internalFormat = GL_RGBA;
			format = GL_RGBA;
		}
		else {
			printf("Unsupported number of channels: %d in %s\n", chan, filename);
			stbi_image_free(pxls);
			return 0;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, pxls);
		glGenerateMipmap(GL_TEXTURE_2D);
		delete[] pxls;
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	printf("Texture: Successfully loaded %s\n", filename);
	return texObject;
}

GLuint setup_mipmaps(const char* filename[], int n)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int w[16], h[16], chan[16];
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pxls[16];

	for (int c=0; c < n; c++)
	{
		pxls[c] = stbi_load(filename[c], &w[c], &h[c], &chan[c], 0);
		if (pxls[c])
		{
			glTexImage2D(GL_TEXTURE_2D, c, GL_RGB, w[c], h[c], 0, GL_RGB, GL_UNSIGNED_BYTE, pxls[c]);
		}
		delete pxls[c];
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	return texObject;
}

