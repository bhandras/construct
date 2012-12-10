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

	private:
		Vector2f mGravity;
		std::vector<Body*> mBodies;
	};
}
