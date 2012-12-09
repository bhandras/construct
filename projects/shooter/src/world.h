#pragma once


class World
{
public:
	World();

	void draw();

private:
	static const int w = 100;
	static const int h = 100;
	char mTiles[w][h];
};
