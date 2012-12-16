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
		~Body();

		Type getType() const;

		void setMass(float mass);
		float getInvMass() const;

		void setShape(Shape* shape);
		Shape* getShape();

		void setPosition(float x, float y);
		void translate(const Vector2f& translation);
		
		void setVelocity(const Vector2f& velocity);
		const Vector2f& getVelocity() const;

		void setElasticity(float elasticity);
		float getElasticity() const;

		void setFriction(float friction);
		float getFriction() const;

		void update(unsigned int deltaTimeMs);

		void contact(Body* body, const Vector2f& pushVector);
		void setContactCallback(IFunctor<void(Body*, const Vector2f&)>* callback);

	protected:
		Type mType;
		Shape* mShape;
		Vector2f mPosition;
		Vector2f mVelocity;

		float mInvMass;
		float mFriction;
		float mElasticity;

		IFunctor<void(Body*, const Vector2f&)>* mContactCallback;
	};
}
