#include "construct.h"
#include "world.h"


using namespace Construct;


World::World()
{
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			mTiles[i][j] = 0;
		}
	}

	for (int i = 10; i < w; ++i)
	{
		mTiles[70][i] = 1;
	}
}


void World::draw()
{
	static const float tileW = 10.0f;
	static const float tileH = 10.0f;

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
					quad.transformation().translation(j * tileW, i * tileH);
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
