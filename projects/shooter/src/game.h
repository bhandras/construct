#pragma once

#include "world.h"
#include "player.h"


class Game
{
public:
	Game();

	void init(unsigned w, unsigned h);
	void step(float deltaTimeMs);

	void testRayCast(float mouseX, float mouseY);
	void playerContact(Construct::Body* body, const Construct::Vector2f& pushVector);
	void jump(float x, float y);

private:
	unsigned mW;
	unsigned mH;

	Construct::TextureAtlas* mAtlas;
	Construct::BitmapFont mFont;

	Player mPlayer;

	// fixed timestep
	float mAccumlator;
	float mTimeStep;

	// physics
	Construct::Space mSpace;
	std::vector<Construct::Body*> mBodies;

	bool mOnTheGround;
	Construct::Circle mCircle;
	Construct::Quad mBoxes[4];

	Construct::Quad mGround;
	Construct::Quad mWalls[2];

	float mFPS;
};
