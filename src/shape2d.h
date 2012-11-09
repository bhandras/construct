#pragma once


#include "color4.h"


class Shape2d
{
public:
	Shape2d();
	virtual ~Shape2d() {}

	void setPosX(int x);
	void setPosX(float x);

	void setPosY(int y);
	void setPosY(float y);

	void setPosition(int x, int y);
	void setPosition(float x, float y);

	void setRotationDeg(float angleDeg);
	void setRotationRad(float angleRad);
	void setScale(float scale);

	float getPosX() const { return mPosX; }
	float getPosY() const { return mPosY; }
	float getRotation() const { return mRotationRad; }
	float getScale() const { return mScale; }
	const Color4& getFillColor() const { return mAreaFillColor; }
	const Color4& getOutlineColor() const { return mOutlineColor; }

	void setFilled(bool isFilled) { mDrawArea = isFilled; }
	void setOutlined(bool isOutlined) { mDrawOutline = isOutlined; }

	void setFillColor(const Color4& color) { mAreaFillColor = color; }
	void setOutlineColor(const Color4& color) { mOutlineColor = color; }
	
	virtual void draw() = 0;

protected:
	bool mDirty;
	float mPosX, mPosY;
	float mRotationRad;
	float mScale;

	bool mDrawArea;
	Color4 mAreaFillColor;
	bool mDrawOutline;
	Color4 mOutlineColor;
};


class RoundedRectangleShape : public Shape2d
{
public:
	enum CornerType
	{
		CORNER_SQUARE	= 0,
		CORNER_ROUND_TL = 1,
		CORNER_ROUND_TR	= 2,
		CORNER_ROUND_BR = 4,
		CORNER_ROUND_BL = 8,
		CORNER_ROUND	= 15
	};

	RoundedRectangleShape() 
		: mW(0.0f)
		, mH(0.0f)
		, mCornerType(CORNER_SQUARE)
		, mCornerRadius(0.0f)
		, mIsShaded(false)
		, mShadeMin(0.0f)
		, mShadeMax(0.0f) {}
	
	void setCornerType(int cornerType) { mCornerType = cornerType; }
	void setCornerRadius(float r) { mCornerRadius = r; }

	void setSize(int w, int h) { mW = static_cast<float>(w); mH = static_cast<float>(h); }
	void setSize(float w, float h) { mW = w; mH = h; }

	void setW(int w) { mW = static_cast<float>(w); }
	void setW(float w) { mW = w; }

	void setH(int h) { mH = static_cast<float>(h); }
	void setH(float h) { mH = h; }

	void setShade(bool isShaded, float shadeMin, float shadeMax) { mIsShaded = isShaded; mShadeMin = shadeMin; mShadeMax = shadeMax; }
	virtual void draw() override;

private:
	void createShade(float* colorArray, int& arraySize);

	float mW, mH;
	int mCornerType;
	float mCornerRadius;
	bool mIsShaded;
	float mShadeMin;
	float mShadeMax;
};


class CircleShape : public Shape2d
{
public:
	CircleShape() : mRadius(0.0f) {}
	void setRadius(float r) { mRadius = r; }

	virtual void draw() override;

private:
	float mRadius;
};


// simple rectangle shape
class RectangleShape : public Shape2d
{
public:
	RectangleShape();
	void setSize(float sizeX, float sizeY);
	void setSizeX(float sizeX);
	void setSizeY(float sizeY);
	virtual void draw() override;

private:
	float mSizeX;
	float mSizeY;
	Vertex_Vector_XYZ_RGBA mVertices;
};


// equilateral triangle shape
class TriangleShape : public Shape2d
{
public:
	TriangleShape();
	void setSize(float size);
	virtual void draw() override;

private:
	float mSize;
	Vertex_Vector_XYZ_RGBA mVertices;
};
