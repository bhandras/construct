#pragma once


namespace Construct
{
	class TextureAtlas
	{
	public:
		TextureAtlas(const std::string& path);

		bool load();
		void unload();
		bool isLoaded() const;

		GL_Texture getTexture() const;

		// return image coordinates inside the texture in range [0, 1] 
		bool getImage(const std::string& name, float& x, float& y, float& w, float& h) const;

	private:
		std::string mPath;
		bool mLoaded;
		GL_Texture mTexture;

		struct ImageRect
		{
			ImageRect()
				: x(0.0f), y(0.0f), w(0.0f), h(0.0f) { }

			float x, y, w, h;
		};

		std::map<std::string, ImageRect> mImageMap;
	};
}
