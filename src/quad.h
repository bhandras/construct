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
class Quad : public Shape2d
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

	enum HotSpot
	{
		QUAD_HOTSPOT_TL,
		QUAD_HOTSPOT_TR,
		QUAD_HOTSPOT_BR,
		QUAD_HOTSPOT_BL,
		QUAD_HOTSPOT_CENTER
	};

	Quad();
	Quad(const Quad& other);
	Quad& operator=(const Quad& other);

	virtual void update(unsigned int deltaTimeMs);

	void setSize(float w, float h);

	/// Set hot spot for rendering
	void setHotSpot(HotSpot hotSpot);

	/// Get the current hot spot
	HotSpot getHotSpot() const { return mHotSpot; }

	/// Get calculated centroid of the box.
	Vector2f getCentroid() const { return mCentroid; }

	/// Get specific corner point of the box.
	const Vector2f& getCorner(CornerIndex index) const { return mCorners[static_cast<int>(index)]; }

	float getW() const { return mSize.w; }
	
	float getH() const { return mSize.h; }

	void setTransformation(const Affine2df& t);

	/// Check if 2d point is inside the box.
	bool isInside(const Vector2f& point) const;

	/// Check for box intersection.
	bool overlaps(const Quad& other) const;

	/// Render the box to OpenGL surface.
	void draw(const Color4& color, float x0, float y0, float x1, float y1) const;

	void intersect(const Vector2f& p0, const Vector2f& p1, Vector2f& result);

private:
	/// Updates the axes after the corners move. Assumes the corners actually form a rectangle.
	void refresh();

	/// Returns true if other overlaps one dimension of this.
	bool overlaps1Way(const Quad& other) const;

	Size2 mSize;
	Affine2df mTransformation;

	HotSpot mHotSpot;

	Vector2f mCorners[4];
	Vector2f mAxis[2];
	float mOrigin[2];
	Vector2f mCentroid;

	Vector2f mNormals[4];
};
