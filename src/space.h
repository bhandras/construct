#pragma once



namespace Construct
{
	class Body;

	class Space
	{
	public:
		~Space();
		Body* addBody(Body::Type bodyType);
		void update(unsigned int deltaTimeMs);

	private:
		std::vector<Body*> mBodies;
	};
}
