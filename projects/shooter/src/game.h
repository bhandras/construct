#pragma once

#include "world.h"
#include "player.h"


class Sandbox
{
public:
	void init();
	void update(unsigned int deltaTimeMs);
	void jump(float x, float y);
	void draw();

private:
	Construct::Quad mBox;
	Construct::Quad mGround;

	Construct::Quad mWalls[2];

	std::vector<Construct::Body*> mBodies;
	Construct::Space mSpace;
};



class Game
{
public:
	Game();

	void init(unsigned w, unsigned h);
	void step(unsigned deltaTimeMs);

private:
	unsigned mW;
	unsigned mH;

	Construct::BitmapFont mFont;
	Construct::TextureAtlas* mAtlas;

	Player mPlayer;
	World mWorld;

	Sandbox mSandbox;
};
