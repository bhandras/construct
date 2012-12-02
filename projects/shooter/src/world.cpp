#include "construct.h"
#include "world.h"


World::World()
{
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			mTiles[i][j] = 0;
		}
	}

	for (int i = 0; i < w; ++i)
	{
		mTiles[h - 1][i] = 1;
	}
}


void World::draw()
{
	Affine2df transform;

	static const int tileW = 15;
	static const int tileH = 15;

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			switch (mTiles[i][j])
			{
			case 1:
				{
					Quad quad;
					quad.setSize(tileW, tileH);
					transform.translation(i * tileW, j * tileH);
					quad.setTransformation(transform);
					quad.update();
					quad.draw();
				}
				break;
			default:
				break;
			}
		}
	}
}
