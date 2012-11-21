#pragma once


class Game
{
public:
	Game();

	void init(unsigned w, unsigned h);
	void step(unsigned mouseX, unsigned mouseY, bool mouseBtn1, bool mouseBtn2, unsigned deltaTimeMs);

private:
	unsigned mW;
	unsigned mH;

	BitmapFont mFont;
	TextureAtlas* mAtlas;

	Sprite mPlayer;
};
