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

	void playerContact(Construct::Body* body, const Construct::Vector2f& pushVector);

private:
	bool mOnTheGround;
	Construct::Quad mBox;
	Construct::Quad mBox2;

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

	Construct::TextureAtlas* mAtlas;

	Player mPlayer;
	World mWorld;

	Sandbox mSandbox;
};
