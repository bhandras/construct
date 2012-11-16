#pragma once 


class Shape2d : public Actor2d
{
public:
	Shape2d()
		: mFillColor(1.0f, 1.0f, 1.0f, 1.0f)
		, mOutlineColor(1.0f, 1.0f, 1.0f, 1.0f)
		, mOutline(false) { }

	virtual ~Shape2d() {}

	// find intersection with line segment (result is the closest to p0)
	virtual bool intersectLineSegment(const Vector2f& p0, const Vector2f& p1, Vector2f& intersection) { return false; }

	const Color4& getFillColor() const { return mFillColor; }
	const Color4& getOutlineColor() const { return mOutlineColor; }

	void setFilled(bool isFilled) { mFill = isFilled; }
	void setOutlined(bool isOutlined) { mOutline = isOutlined; }

	void setFillColor(const Color4& color) { mFillColor = color; }
	void setOutlineColor(const Color4& color) { mOutlineColor = color; }
	
	virtual void draw() = 0;

protected:
	bool mFill;
	Color4 mFillColor;
	bool mOutline;
	Color4 mOutlineColor;
};
