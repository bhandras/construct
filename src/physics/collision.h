#pragma once


namespace Construct
{
	class Collision2d
	{
	public:
		static bool intersectLineSegments(const Vector2f& a0, const Vector2f& a1, const Vector2f& b0, const Vector2f& b1, Vector2f& intersection);
		static bool intersectPolygons(const std::vector<Vector2f>& edgesA, const Vector2f& positionA, const std::vector<Vector2f>& edgesB, const Vector2f& positionB, Vector2f& pushVector);
	};
}
