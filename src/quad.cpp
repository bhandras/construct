#include "construct.h"


Quad::Quad()
{
	mEdges.resize(4);
	mNormals.resize(4);
}


Quad::Quad(const Quad& other)
{
	mSize = other.mSize;
	mEdges.resize(4);
	mNormals.resize(4);

	mEdges = other.mEdges;
}


Quad& Quad::operator=(const Quad& other)
{
	if (this != &other)
	{
		mSize = other.mSize;
		mEdges = other.mEdges;
	}

	return *this;
}


void Quad::update(unsigned int deltaTimeMs)
{
	// create an axis aligned box first
	Vector2f x(mSize.w, 0.0f);
	Vector2f y(0.0f, mSize.h);

	mEdges[QUAD_CORNER_TL].x = 0.0f;
	mEdges[QUAD_CORNER_TL].y = 0.0f;
	mEdges[QUAD_CORNER_TR] = x;
	mEdges[QUAD_CORNER_BR] = x + y;
	mEdges[QUAD_CORNER_BL] = y;
	mCentroid = (mEdges[0] + mEdges[1] + mEdges[2] + mEdges[3]) * 0.25f;

	mTransformation.transform(mCentroid);
	
	// create corners
	for (int i = 0; i < 4; ++i)
	{
		mTransformation.transform(mEdges[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		mNormals[i] = mEdges[(i + 1) % 4] - mEdges[i];
		mNormals[i].normalize();

		float tmp = mNormals[i].x;
		mNormals[i].x = mNormals[i].y;
		mNormals[i].y = -tmp;
	}
}


void Quad::setSize(float w, float h)
{
	mSize.w = w;
	mSize.h = h;
}


void Quad::setTransformation(const Affine2df& t)
{
	mTransformation = t;
}


void Quad::intersect(const Vector2f& p0, const Vector2f& p1, Vector2f& result)
{
	Vector2f v = p1 - p0;
	v.normalize();

	for (int i = 0; i < 4; i += 1)
	{
		int j = (i + 1) % 4;
		float dot = v.dot(mNormals[i]);
		if (dot < 0.0f && Intersection2d::line(mEdges[i], mEdges[j], p0, p1, result))
		{
			return;
		}
	}

	result = p1;
}


void Quad::draw()
{
	Vertex_Vector_XYZ_RGBA vertices;
	vertices.resize(4);

	vertices[0].setPosition(mEdges[0]);
	vertices[1].setPosition(mEdges[1]);
	vertices[2].setPosition(mEdges[2]);
	vertices[3].setPosition(mEdges[3]);

	Index_Vector indices;
	indices.resize(6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	GL_Render& gl = GL_Render::get();

	if (Context::isFilled())
	{
		const Color4 c = Context::getFillColor();
		vertices[0].setColor(c);
		vertices[1].setColor(c);
		vertices[2].setColor(c);
		vertices[3].setColor(c);

		gl.setDrawMode(GL_TRIANGLES);
		gl.draw_XYZ_RGBA(vertices, indices);
	}

	if (Context::isOutlined())
	{
		const Color4 c = Context::getOutlineColor();
		vertices[0].setColor(c);
		vertices[1].setColor(c);
		vertices[2].setColor(c);
		vertices[3].setColor(c);

		gl.setDrawMode(GL_LINE_LOOP);
		gl.draw_XYZ_RGBA(vertices, indices);
	}
}
