#pragma once


class Player
{
public:
	enum Direction
	{
		D_LEFT,
		D_RIGHT
	};

	Player();
	void init();

	void setPosition(float x, float y);
	void move(Direction direction);
	void jump();

	void update(unsigned int deltaTimeMs);
	void draw();

private:
	Direction mDirection;
	float mVelocityX;
	float mVelocityY;

	Construct::TextureAtlas mSpriteAtlas;
	Construct::Sprite mPlayerSprite;
};