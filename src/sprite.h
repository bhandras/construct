#pragma once


class Sprite : public Actor2d
{
public:
	Sprite();
	virtual ~Sprite();
	
	void setAtlas(const TextureAtlas* atlas);
	void addKeyFrameImage(const std::string& anim, const std::string& name);

	void selectAnimation(const std::string& anim);
	void setFPS(int FPS);
	void setLooped(bool isLooped);

	void update(unsigned int deltaTimeMs);
	void draw();

private:
	const TextureAtlas* mAtlas;
	struct KeyFrame
	{
		float u0, v0, u1, v1;
		float u2, v2, u3, v3;

		Quad quad;
	};

	typedef std::vector<KeyFrame*> KeyFrameArray;
	typedef std::map<std::string, KeyFrameArray> AnimationMap;
	
	AnimationMap mAnimationMap;
	std::string mCurrentAnimation;
	size_t mCurrentKeyFrame;
	unsigned int mTickMs;

	int mFPS;
	bool mIsLooped;
	Color4 mColor;
};
