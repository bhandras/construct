#pragma once


class Polygon
{
public:
	bool intersects(const Polygon& other);

	// find intersection with line segment (result is the closest to p0)
	bool intersectLineSegment(const Vector2f& p0, const Vector2f& p1, Vector2f& intersection) { return false; }

private:
	static void calculateProjectedInterval(const Polygon& p, const Vector2f& axis, float& a, float& b);

protected:
	Vector2f mCentroid;
	std::vector<Vector2f> mEdges;
	std::vector<Vector2f> mNormals;
	Affine2df mTransformation;
};
