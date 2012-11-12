#pragma once 


class Shape2d : public Actor2d
{
public:
	Shape2d()
		: mAreaFillColor(1.0f, 1.0f, 1.0f, 1.0f)
		, mOutlineColor(1.0f, 1.0f, 1.0f, 1.0f)
		, mDrawOutline(false) { }

	virtual ~Shape2d() {}

	const Color4& getFillColor() const { return mAreaFillColor; }
	const Color4& getOutlineColor() const { return mOutlineColor; }

	void setFilled(bool isFilled) { mDrawArea = isFilled; }
	void setOutlined(bool isOutlined) { mDrawOutline = isOutlined; }

	void setFillColor(const Color4& color) { mAreaFillColor = color; }
	void setOutlineColor(const Color4& color) { mOutlineColor = color; }
	
	virtual void draw() = 0;

protected:
	bool mDrawArea;
	Color4 mAreaFillColor;
	bool mDrawOutline;
	Color4 mOutlineColor;
};
