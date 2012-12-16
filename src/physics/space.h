#pragma once



namespace Construct
{
	class Body;

	class Space
	{
	public:
		~Space();
		void setGravity(const Vector2f& gravity);
		Body* addBody(Body::Type bodyType);
		void update(unsigned int deltaTimeMs);
		void debug();

	private:
		void handleCollision(Body* body1, Body* body2, const Vector2f& minSeparation);
		Vector2f calculateVelocity(const Vector2f& v, const Vector2f& n, float elasticity, float friction) const;

		Vector2f mGravity;
		std::vector<Body*> mBodies;
		std::list<Contact> contacts;
	};
}
