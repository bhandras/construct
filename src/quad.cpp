#include "construct.h"


namespace Construct
{
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

		Polygon::update();
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
			if (dot < 0.0f && Collision2d::intersectLineSegments(mEdges[i], mEdges[j], p0, p1, result))
			{
				return;
			}
		}

		result = p1;
	}
}
