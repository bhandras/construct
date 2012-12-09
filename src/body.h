#pragma once


namespace Construct
{
	class Body
	{
	public:
		Body();
		void setShape(Shape* shape);
		Shape* getShape();

		void translate(const Vector2f& translation);
		
		void setVelocity(const Vector2f& velocity);
		const Vector2f& getVelocity();

		void update(unsigned int deltaTimeMs);

	protected:
		Shape* mShape;
		Vector2f mPosition;
		Vector2f mVelocity;

		float mInvMass;
		float mFriction;
		float mElasticity;
	};
}
