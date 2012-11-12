#pragma once


class GL_Texture
{
public:
	GLuint getW() const { return mWidth; }
	GLuint getH() const { return mHeight; }
	GLuint getId() const { return mId; }
	std::string getPath() const { return mPath; }

private:
	// GL_TextureManager can access/fill members
	friend class GL_TextureManager;
	std::string mPath;

	GLuint mWidth;
	GLuint mHeight;
	GLuint mId;
};


class GL_TextureManager
{
public:
	static GL_TextureManager& get()
	{
		static GL_TextureManager instance;
		return instance;
	}
	
	bool loadTexture(const std::string& path);
	GL_Texture acquireTexture(const std::string& path);
	void releaseTexture(const std::string& path);

private:
	GL_TextureManager(unsigned int maxMemory = MAX_OPENGL_TEXTURE_MEMORY);
	~GL_TextureManager();
	
	// don't copy
	GL_TextureManager(const GL_TextureManager&);
	GL_TextureManager& operator=(const GL_TextureManager&);


	struct TextureData
	{
		GL_Texture texture;
		unsigned int refCount;
	};
	
	unsigned int mMaxMemory;
	unsigned int mUsedMemory;

	std::map<std::string, TextureData> mTextures;
};
