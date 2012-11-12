#include "construct.h"


bool Intersection2d::line(const Vector2f& a0, const Vector2f& a1, const Vector2f& b0, const Vector2f& b1, Vector2f& intersection)
{
	Vector2f da = a1 - a0;
	Vector2f db = b1 - b0;
	
	float da_cross_db = da.cross(db);

	// b cross d == 0 means that the lines are parallel
	if (fabs(da_cross_db) < NumericTraits<float>::eps())
	{
		return false;
	}

	Vector2f c = b0 - a0;

	float t = c.cross(db) / da_cross_db;
	
	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}

	float u = c.cross(da) / da_cross_db;

	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	intersection = a0 + da * t;

	return true;
}
