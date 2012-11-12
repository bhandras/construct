#include "construct.h"


TextureAtlas::TextureAtlas(const std::string& path)
: mPath(path)
, mLoaded(false)
{ }


bool TextureAtlas::load()
{
	if (mLoaded)
	{
		return true;
	}

	XML::Document xml(mPath);

	if (!xml.isValid())
	{
		return false;
	}

	XML::Element e = xml.getElement("atlas");
	std::string texturePath = e.attribute("texture");
	GL_TextureManager::get().loadTexture(texturePath);
	mTexture = GL_TextureManager::get().acquireTexture(texturePath);

	for (e = e.first(); e.isValid(); e = e.next())
	{
		std::string key = e.name();

		if (key.compare("image") == 0)
		{
			ImageRect imageRect;
			e.attribute("x", imageRect.x);
			e.attribute("y", imageRect.y);
			e.attribute("w", imageRect.w);
			e.attribute("h", imageRect.h);
			mImageMap[e.attribute("name")] = imageRect;
		}
	}

	mLoaded = true;
	return true;

}


void TextureAtlas::unload()
{
	if (mLoaded)
	{
		GL_TextureManager::get().releaseTexture(mTexture.getPath());
		mImageMap.clear();
		mLoaded = false;
	}
}


bool TextureAtlas::isLoaded() const
{
	return mLoaded;
}


GL_Texture TextureAtlas::getTexture() const
{
	return mTexture;
}


bool TextureAtlas::getImage(const std::string& name, float& x, float& y, float& w, float& h) const
{
	std::map<std::string, ImageRect>::const_iterator it = mImageMap.find(name);
	if (it != mImageMap.end())
	{
		x = it->second.x;
		y = it->second.y;
		w = it->second.w;
		h = it->second.h;

		return true;
	}

	return false;
}
