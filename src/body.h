#pragma once


namespace Construct
{
	class Body
	{
	public:
		enum Type
		{
			StaticBody,
			DynamicBody
		};

		Body(Type type);

		Type getType() const;

		void setShape(Shape* shape);
		Shape* getShape();

		void translate(const Vector2f& translation);
		
		void setVelocity(const Vector2f& velocity);
		const Vector2f& getVelocity();

		void update(unsigned int deltaTimeMs);

	protected:
		Type mType;
		Shape* mShape;
		Vector2f mPosition;
		Vector2f mVelocity;

		float mFriction;
		float mElasticity;
	};
}
