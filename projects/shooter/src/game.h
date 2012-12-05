#pragma once

#include "world.h"
#include "player.h"


class Game
{
public:
	Game();

	void init(unsigned w, unsigned h);
	void step(unsigned deltaTimeMs);

private:
	unsigned mW;
	unsigned mH;

	BitmapFont mFont;
	TextureAtlas* mAtlas;

	Player mPlayer;
	World mWorld;
};
