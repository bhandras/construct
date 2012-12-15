#pragma once


namespace Construct
{
	class Distance2d
	{
	public:
		static Vector2f closestPointOnLineSegment(const Vector2f& p, const Vector2f& a, const Vector2f& b);
		static Vector2f closestPointOnTriangle(const Vector2f& p, const Vector2f& a, const Vector2f& b, const Vector2f& c, int* region, int& regionCount);
		static Vector2f pointConvexSet(const Vector2f& p, const std::vector<Vector2f>& convexSet);
	};
}
