#pragma once

#include <cassert>
#include "math/math.h"


// 2d size container
struct Size2
{
	Size2() : w(0.0f), h(0.0f) {}
	Size2(float _w, float _h) : w(_w), h(_h) {}

	/// Common helper constructors to avoit casting in the code.
	Size2(int _w, int _h) : w(static_cast<float>(_w)), h(static_cast<float>(_h)) {}
	Size2(unsigned _w, unsigned _h) : w(static_cast<float>(_w)), h(static_cast<float>(_h)) {}

	float w;
	float h;
};


/// 2d oriented bounding box
class Quad
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

	/// Default constructor.
	Quad();

	/// Copy constructor.
	Quad(const Quad& other);

	/// Assignment operator.
	Quad& operator=(const Quad& other);

	/// Reset the box to its original position + rotation.
	void update();

	void setSize(float w, float h);

	/// Set hot spot for rendering
	void setHotSpot(HotSpot hotSpot);

	/// Get the current hot spot
	HotSpot getHotSpot() const { return mHotSpot; }

	/// Get calculated centroid of the box.
	Vector2f getCentroid() const { return mCentroid; }

	/// Get specific corner point of the box.
	const Vector2f& getCorner(CornerIndex index) const { return mCorners[static_cast<int>(index)]; }

	const Vector2f& getPosition() const { return mPosition; }

	/// Move the box.
	void setPosition(const Vector2f& position);

	float getW() const { return mSize.w * mScale; }
	
	float getH() const { return mSize.h * mScale; }

	/// Reset rotation to given angle (in radians) and rotate the box around its centroid.
	void setRotationDeg(float angleDeg);

	/// Set the scale of the rect (scale is used for centered scaling)
	void setScale(float scale);

	/// Check if 2d point is inside the box.
	bool isInside(const Vector2f& point) const;

	/// Check for box intersection.
	bool overlaps(const Quad& other) const;

	/// Render the box to OpenGL surface.
	void draw(const Color4& color) const;

private:
	/// Updates the axes after the corners move. Assumes the corners actually form a rectangle.
	void refresh();

	/// Returns true if other overlaps one dimension of this.
	bool overlaps1Way(const Quad& other) const;

	Vector2f mPosition;
	Size2 mSize;
	float mRotationRad;
	float mScale;
	Affine2df mTransformation;

	HotSpot mHotSpot;

	Vector2f mCorners[4];
	Vector2f mAxis[2];
	float mOrigin[2];
	Vector2f mCentroid;
};
