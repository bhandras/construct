#include "construct.h"


bool Intersection2d::line(const Vector2f& a1, const Vector2f& a2, const Vector2f& b1, const Vector2f& b2, Vector2f& intersection)
{
	float s1_x = a2.x - a1.x;
	float s1_y = a2.y - a1.y;

	float s2_x = b2.x - b1.x;
	float s2_y = b2.y - b1.y;

	float s = (-s1_y * (a1.x - b1.x) + s1_x * (a1.y - b1.y)) / (-s2_x * s1_y + s1_x * s2_y);
	float t = ( s2_x * (a1.y - b1.y) - s2_y * (a1.x - b1.x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		intersection.x = a1.x + (t * s1_x);
		intersection.y = a1.y + (t * s1_y);
		return true;
	}

	return false;
}
