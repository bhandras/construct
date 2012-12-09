#pragma once



namespace Construct
{
	class Body;

	class Space
	{
	public:
		~Space();
		Body* addBody();
		void update(unsigned int deltaTimeMs);

	private:
		std::vector<Body*> mBodies;
	};
}
