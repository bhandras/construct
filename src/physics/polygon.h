#pragma once


namespace Construct
{
	class Polygon : public Shape
	{
	public:
		Polygon();
		const std::vector<Vector2f>& edges() const { return mEdges; }
		const std::vector<Vector2f>& normals() const { return mNormals; }

		virtual void update();
		virtual void draw();

	protected:
		std::vector<Vector2f> mEdges;
		std::vector<Vector2f> mNormals;
	};
}
