#include "construct.h"


bool Polygon::intersects(const Polygon& other)
{
	for (size_t i = 0; i < mEdges.size() - 1; ++i)
	{
		Vector2f& v0 = mEdges[i];
		Vector2f& v1 = mEdges[i + 1];

		// normal of ab
		Vector2f n(-(v1.y - v0.y), v1.x - v0.x);

		float a0, a1;
		calculateProjectedInterval(*this, n, a0, a1);

		float b0, b1;
		calculateProjectedInterval(other, n, b0, b1);

		bool overlap = (b1 > a0 && a0 > b0) || (b1 > a1 && a1 > b0) || (a0 < b0 && a1 > b1);

		if (overlap)
		{
			return true;
		}
	}

	return false;
}


void Polygon::calculateProjectedInterval(const Polygon& p, const Vector2f& axis, float& a, float& b)
{
	a = b = p.mEdges[0].dot(axis);

	for (size_t i = 1; i < p.mEdges.size(); ++i)
	{
		float x = p.mEdges[0].dot(axis);

		if (x < a)
		{
			a = x;
		}
		else if (x > b)
		{
			b = x;
		}
	}
}
