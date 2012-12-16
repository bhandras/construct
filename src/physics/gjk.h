#pragma once


namespace Construct
{
	class Polygon;

	class GJK2d
	{
	public:
		static void distance(const Polygon& polygon1, const Polygon& polygon2, Vector2f& witnessPoint1, Vector2f& witnessNormal1, Vector2f& witnessPoint2, Vector2f& witnessNormal2);

	private:
		struct Vertex
		{
			// index of p1 and p2
			size_t index1, index2;

			// support from polygon 1
			Vector2f p1;

			// support from polygon 2
			Vector2f p2;

			// Minkowski sum for p2 and p1 = p2 - p1
			Vector2f p;

			// barycentric coordinate for the closest point
			float u;
		};


		struct Simplex
		{
			Simplex() : count(0) {}
			
			Vertex vertices[3];

			// vertex count
			size_t count;

			// divisor for the barycentric coordinates
			float divisor;
		};

		// find the closest point to p in the simplex
		static void solveSimplex(const Vector2f& p, Simplex& simplex);
		static size_t findSupportPoint(const Vector2f& d, const std::vector<Vector2f>& points);
		static Vector2f getSearchDirection(const Simplex& simplex);
		static void getWitnessPoints(const Simplex& simplex, Vector2f& point1, Vector2f& normal1, Vector2f& point2, Vector2f& normal2);
	};
}