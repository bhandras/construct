#pragma once


namespace Construct
{
	class Shape
	{
	public:
		enum Type
		{
			CircleShape,
			SegmentShape,
			PolygonShape
		};

		Shape(Type type) : mType(type) {}
		virtual ~Shape() {}
		Type getType() { return mType; }
		Affine2df& transformation() { return mTransformation; }

	protected:
		Type mType;
		Affine2df mTransformation;
	};
}
