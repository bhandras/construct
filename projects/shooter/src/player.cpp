#include "construct.h"
#include "player.h"


Player::Player()
: mDirection(D_RIGHT) 
, mVelocityX(0.0f)
, mVelocityY(0.0f)
, mSpriteAtlas("player.xml")
{ }


void Player::init()
{
	mSpriteAtlas.load();
	mPlayerSprite.setAtlas(&mSpriteAtlas);

	mPlayerSprite.addKeyFrameImage("stand", "stand0");
	mPlayerSprite.addKeyFrameImage("stand", "stand1");
	mPlayerSprite.addKeyFrameImage("stand", "stand2");
	mPlayerSprite.addKeyFrameImage("stand", "stand3");
	mPlayerSprite.addKeyFrameImage("stand", "stand4");

	mPlayerSprite.addKeyFrameImage("walk", "walk0");
	mPlayerSprite.addKeyFrameImage("walk", "walk1");
	mPlayerSprite.addKeyFrameImage("walk", "walk2");
	mPlayerSprite.addKeyFrameImage("walk", "walk3");
	mPlayerSprite.addKeyFrameImage("walk", "walk4");

	mPlayerSprite.addKeyFrameImage("jump", "jump0");
	mPlayerSprite.addKeyFrameImage("jump", "jump1");
	mPlayerSprite.addKeyFrameImage("jump", "jump2");

	mPlayerSprite.selectAnimation("walk");

	mPlayerSprite.setFPS(5);
	mPlayerSprite.setScale(5, 5);
	mPlayerSprite.setLooped(true);
}


void Player::setPosition(float x, float y)
{
	mPlayerSprite.setPosition(x, y);
}


void Player::move(Direction direction)
{
	static const float moveSpeed = 1;
	static const float maxSpeed = 10;
	if (mDirection != direction)
	{
		mVelocityX = 0.0f;
		mDirection = direction;
	}

	mVelocityX += (direction == D_RIGHT) ? moveSpeed : -moveSpeed;
	mVelocityX *= 1.1f;
	if (mVelocityX > maxSpeed)
	{
		mVelocityX = maxSpeed;
	}

	if (mVelocityX < -maxSpeed)
	{
		mVelocityX = -maxSpeed;
	}
}


void Player::jump()
{

}


void Player::update(unsigned int deltaTimeMs)
{
	float deltaTimeSec = deltaTimeMs / 1000.0f;

	float x, y;
	mPlayerSprite.getPosition(x, y);
	x += mVelocityX * deltaTimeSec;
	y += mVelocityY * deltaTimeSec;
	
	mVelocityX /= 1.1f;
	if (fabs(mVelocityX) < 0.01f)
	{
		mVelocityX = 0.0f;
	}

	mVelocityY += 100 * deltaTimeSec;

	mPlayerSprite.setPosition(x, y);
	mPlayerSprite.update(deltaTimeMs);
}


void Player::draw()
{
	mPlayerSprite.draw();
}
