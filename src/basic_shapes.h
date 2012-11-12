#pragma once


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
};


// 
class CircleShape : public Shape2d
{
public:
	CircleShape() : mRadius(0.0f) {}
	void setRadius(float r) { mRadius = r; }

	virtual void draw() override;

private:
	float mRadius;
};
