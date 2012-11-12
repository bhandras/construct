#include "construct.h"


bool Intersection2d::line(const Vector2f& a1, const Vector2f& a2, const Vector2f& b1, const Vector2f& b2, Vector2f& intersection)
{
	Vector2f a = a2 - a1;
	Vector2f b = b2 - b1;
	
	float f = a.cross(b);

	// b cross d == 0 means that the lines are parallel
	if (fabs(f) < NumericTraits<float>::eps())
	{
		return false;
	}

	Vector2f c = b2 - a2;
	float aa = a.cross(c);
	float bb = b.cross(c);

	if(f < 0)
	{
		if(aa > 0)     return false;
		if(bb > 0)     return false;
		if(aa < f)     return false;
		if(bb < f)     return false;
	}
	else
	{
		if(aa < 0)     return false;
		if(bb < 0)     return false;
		if(aa > f)     return false;
		if(bb > f)     return false;
	}

	float t = 1.0f - (aa / f);
	intersection = a1 + a * t;

	return true;
}
