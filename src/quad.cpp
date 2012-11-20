#include "construct.h"


Quad::Quad()
{
	mEdges.resize(4);
	mNormals.resize(4);
}


void Quad::update()
{
	float w2 = mSize.w / 2.0f;
	float h2 = mSize.h / 2.0f;

	mEdges[QUAD_EDGE_TL].x = -w2;
	mEdges[QUAD_EDGE_TL].y = -h2;

	mEdges[QUAD_EDGE_TR].x = w2;
	mEdges[QUAD_EDGE_TR].y = -h2;

	mEdges[QUAD_EDGE_BR].x = w2;
	mEdges[QUAD_EDGE_BR].y = h2;

	mEdges[QUAD_EDGE_BL].x = -w2;
	mEdges[QUAD_EDGE_BL].y = h2;
	
	// create corners
	for (int i = 0; i < 4; ++i)
	{
		mTransformation.transform(mEdges[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		mNormals[i] = mEdges[(i + 1) % 4] - mEdges[i];
		mNormals[i].normalize();

		float tmp = mNormals[i].x;
		mNormals[i].x = mNormals[i].y;
		mNormals[i].y = -tmp;
	}
}


void Quad::setSize(float w, float h)
{
	mSize.w = w;
	mSize.h = h;
}


void Quad::intersect(const Vector2f& p0, const Vector2f& p1, Vector2f& result)
{
	Vector2f v = p1 - p0;
	v.normalize();

	for (int i = 0; i < 4; i += 1)
	{
		int j = (i + 1) % 4;
		float dot = v.dot(mNormals[i]);
		if (dot < 0.0f && Intersection2d::line(mEdges[i], mEdges[j], p0, p1, result))
		{
			return;
		}
	}

	result = p1;
}
