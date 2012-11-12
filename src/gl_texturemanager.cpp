#include "construct.h"
#include "3rdparty/lodepng/lodepng.h"

// more OpenGL info here: http://iphonedevelopment.blogspot.com/2009/05/opengl-es-from-ground-up-part-6_25.html


GL_TextureManager::GL_TextureManager(unsigned int MaxMemory)
: mMaxMemory(MaxMemory)
, mUsedMemory(0)
{ }


GL_TextureManager::~GL_TextureManager()
{
	// todo
}


bool GL_TextureManager::loadTexture(const std::string& path)
{
	if (mTextures.size() > 0 && mTextures.find(path) != mTextures.end())
	{
		// already loaded
		return true;
	}

	std::vector<unsigned char> buffer, image;
	LodePNG::loadFile(buffer, path.c_str());
	LodePNG::Decoder decoder;
	decoder.decode(image, buffer.empty() ? 0 : &buffer[0], (unsigned)buffer.size());

	if (decoder.hasError())
	{
		return false;
	}

	TextureData textureData;
	GL_Texture& texture = textureData.texture;
	texture.mWidth = decoder.getWidth();
	texture.mHeight = decoder.getHeight();

	glGenTextures(1, &texture.mId);
	glBindTexture(GL_TEXTURE_2D, texture.mId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(texture.mWidth), static_cast<GLsizei>(texture.mHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	mTextures[path] = textureData;
	return true;
}


GL_Texture GL_TextureManager::acquireTexture(const std::string& path)
{
	TextureData& textureData = mTextures[path];
	textureData.refCount++;

	return textureData.texture;
}


void GL_TextureManager::releaseTexture(const std::string& path)
{
	std::map<std::string, TextureData>::iterator it = mTextures.find(path);
	if (it != mTextures.end())
	{
		TextureData& textureData = it->second;
		textureData.refCount--;

		if (textureData.refCount == 0)
		{
			glDeleteTextures(1, &textureData.texture.mId);
			mTextures.erase(it);
		}
	}
}
