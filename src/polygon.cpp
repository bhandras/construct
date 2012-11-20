#include "construct.h"


void Polygon::setTransformation(const Affine2df& t)
{
	mTransformation = t;
}


bool Polygon::intersects(const Polygon& other)
{
	for (size_t i = 0; i < mEdges.size() - 1; ++i)
	{
		Vector2f& v0 = mEdges[i];
		Vector2f& v1 = mEdges[i + 1];

		// normal of ab
		Vector2f n(-(v1.y - v0.y), v1.x - v0.x);

		float a0, a1;
		calculateProjectedInterval(*this, n, a0, a1);

		float b0, b1;
		calculateProjectedInterval(other, n, b0, b1);

		bool overlap = (b1 > a0 && a0 > b0) || (b1 > a1 && a1 > b0) || (a0 < b0 && a1 > b1);

		if (overlap)
		{
			return true;
		}
	}

	return false;
}


void Polygon::calculateProjectedInterval(const Polygon& p, const Vector2f& axis, float& a, float& b)
{
	a = b = p.mEdges[0].dot(axis);

	for (size_t i = 1; i < p.mEdges.size(); ++i)
	{
		float x = p.mEdges[0].dot(axis);

		if (x < a)
		{
			a = x;
		}
		else if (x > b)
		{
			b = x;
		}
	}
}


void Polygon::draw()
{
	Vertex_Vector_XYZ_RGBA vertices;
	vertices.resize(mEdges.size());

	for (size_t i = 0; i < mEdges.size(); ++i)
	{
		vertices[i].setPosition(mEdges[i]);
	}

	Index_Vector indices;
	indices.resize((mEdges.size() - 2) * 3);

	size_t j = 1; 
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		indices[i] = 0;
		indices[i + 1] = j;
		indices[i + 2] = j + 1;
		++j;
	}

	GL_Render& gl = GL_Render::get();

	if (Context::isFilled())
	{
		const Color4 c = Context::getFillColor();
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			vertices[i].setColor(c);
		}

		gl.setDrawMode(GL_TRIANGLES);
		gl.draw_XYZ_RGBA(vertices, indices);
	}

	if (Context::isOutlined())
	{
		const Color4 c = Context::getOutlineColor();
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			vertices[i].setColor(c);
		}

		gl.setDrawMode(GL_LINE_LOOP);
		gl.draw_XYZ_RGBA(vertices, indices);
	}
}
