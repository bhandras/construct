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

		void setShape(Shape* shape);
		Shape* getShape();

		void setPosition(float x, float y);
		void translate(const Vector2f& translation);
		
		void setVelocity(const Vector2f& velocity);
		const Vector2f& getVelocity();

		void update(unsigned int deltaTimeMs);

		void contact(Body* body);
		void setContactCallback(IFunctor<void(Body*)>* callback);

	protected:
		Type mType;
		Shape* mShape;
		Vector2f mPosition;
		Vector2f mVelocity;

		float mFriction;
		float mElasticity;

		IFunctor<void(Body*)>* mContactCallback;
	};
}
