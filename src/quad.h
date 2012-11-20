#pragma once


// 2d size container
struct Size2
{
	Size2()
		: w(0.0f), h(0.0f) {}
	
	Size2(float _w, float _h)
		: w(_w), h(_h) {}

	/// Common helper constructors to avoit casting in the code.
	Size2(int _w, int _h)
		: w(static_cast<float>(_w)), h(static_cast<float>(_h)) {}
	
	Size2(unsigned _w, unsigned _h)
		: w(static_cast<float>(_w)), h(static_cast<float>(_h)) {}

	float w;
	float h;
};


/// 2d oriented bounding box
class Quad : public Polygon
{
public:
	///
	enum CornerIndex
	{
		QUAD_CORNER_TL = 0,
		QUAD_CORNER_TR = 1,
		QUAD_CORNER_BR = 2,
		QUAD_CORNER_BL = 3
	};

	Quad();
	Quad(const Quad& other);
	Quad& operator=(const Quad& other);

	virtual void update(unsigned int deltaTimeMs);

	void setSize(float w, float h);

	/// Get calculated centroid of the box.
	Vector2f getCentroid() const { return mCentroid; }

	/// Get specific corner point of the box.
	const Vector2f& getCorner(CornerIndex index) const { return mEdges[static_cast<int>(index)]; }

	float getW() const { return mSize.w; }
	
	float getH() const { return mSize.h; }

	void setTransformation(const Affine2df& t);

	/// Check if 2d point is inside the box.
	bool isInside(const Vector2f& point) const;

	/// Check for box intersection.
	bool overlaps(const Quad& other) const;

	/// Render the box to OpenGL surface.
	virtual void draw();

	void intersect(const Vector2f& p0, const Vector2f& p1, Vector2f& result);

private:
	Size2 mSize;
};
