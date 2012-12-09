#pragma once


namespace Construct
{
	class Polygon : public Shape
	{
	public:
		Polygon();
		void setTransformation(const Affine2df& t);

		const std::vector<Vector2f>& edges() const { return mEdges; }
		Vector2f translation() { return mTransformation.translation(); }

		virtual void update() {};
		virtual void draw();

	protected:
		std::vector<Vector2f> mEdges;
		std::vector<Vector2f> mNormals;
		Affine2df mTransformation;
	};
}