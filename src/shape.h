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

	protected:
		Type mType;
	};

}
