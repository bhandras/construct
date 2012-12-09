#pragma once


namespace Construct
{
	// 2d size container
	struct Size2
	{
		Size2()
			: w(0.0f), h(0.0f) {}

		Size2(float _w, float _h)
			: w(_w), h(_h) {}

		/// Common helper constructors to avoit casting in the code.
		Size2(int _w, int _h)
			: w(static_cast<float>(_w)), h(static_cast<float>(_h)) {}

		Size2(unsigned _w, unsigned _h)
			: w(static_cast<float>(_w)), h(static_cast<float>(_h)) {}

		float w;
		float h;
	};


	/// 2d oriented bounding box
	class Quad : public Polygon
	{
	public:
		///
		enum EdgeIndex
		{
			QUAD_EDGE_TL = 0,
			QUAD_EDGE_TR = 1,
			QUAD_EDGE_BR = 2,
			QUAD_EDGE_BL = 3
		};

		Quad();

		void setSize(float w, float h);
		float getW() const { return mSize.w; }
		float getH() const { return mSize.h; }
		const Vector2f& getEdge(EdgeIndex index) const { return mEdges[static_cast<int>(index)]; }

		virtual void update();

		void intersect(const Vector2f& p0, const Vector2f& p1, Vector2f& result);

	private:
		Size2 mSize;
	};
}
