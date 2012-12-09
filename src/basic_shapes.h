#pragma once


namespace Construct
{
	// equilateral triangle shape
	class Triangle : public Polygon
	{
	public:
		Triangle();

		void setSize(float size);
		virtual void update() override;

	private:
		float mSize;
	};


	// 
	class Circle : public Polygon
	{
	public:
		Circle(int numSlices = 16);
		void setRadius(float r) { mRadius = r; }

		virtual void update() override;

	private:
		float mRadius;
	};
}
