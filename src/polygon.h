#pragma once


class Polygon
{
public:
	void setTransformation(const Affine2df& t);

	bool intersects(const Polygon& other, Vector2f& pushVector);

	// find intersection with line segment (result is the closest to p0)
	bool intersectLineSegment(const Vector2f& p0, const Vector2f& p1, Vector2f& intersection) { return false; }

	virtual void update() {};
	virtual void draw();

private:
	static bool axisSeparatesPolygons(Vector2f& axis, const Polygon& p1, const Polygon& p2);
	static void calculateProjectedInterval(const Polygon& p, const Vector2f& axis, float& a, float& b);

protected:
	std::vector<Vector2f> mEdges;
	std::vector<Vector2f> mNormals;
	Affine2df mTransformation;
};
