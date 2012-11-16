#include "construct.h"


RectangleShape::RectangleShape()
: mSizeX(0.0f)
, mSizeY(0.0f)
{ }


void RectangleShape::setSize(float sizeX, float sizeY)
{
	mSizeX = sizeX;
	mSizeY = sizeY;
}


void RectangleShape::setSizeX(float sizeX)
{
	mSizeX = sizeX;
}


void RectangleShape::setSizeY(float sizeY)
{
	mSizeY = sizeY;
}


void RectangleShape::draw()
{
	Vertex_Vector_XYZ_RGBA vertices;
	vertices.resize(4);

	float sizeX2 = mSizeX / 2.0f;
	float sizeY2 = mSizeY / 2.0f;

	Vector2f edges[4];
	// bl
	edges[0].x = -sizeX2;
	edges[0].y = -sizeY2;

	// br
	edges[1].x = sizeX2;
	edges[1].y = -sizeY2;

	// tr
	edges[2].x = sizeX2;
	edges[2].y = sizeY2;

	// tl
	edges[3].x = -sizeX2;
	edges[3].y = sizeY2;

	for (int i = 0; i < 4; ++i)
	{
		mTransformation.transform(edges[i]);
		vertices[i].setPosition(edges[i]);
	}

	Index_Vector indices;
	indices.resize(6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	GL_Render& gl = GL_Render::get();

	if (mFill)
	{
		vertices[0].setColor(mFillColor);
		vertices[1].setColor(mFillColor);
		vertices[2].setColor(mFillColor);
		vertices[3].setColor(mFillColor);

		gl.setDrawMode(GL_TRIANGLES);
		gl.draw_XYZ_RGBA(vertices, indices);
	}

	if (mOutline)
	{
		vertices[0].setColor(mOutlineColor);
		vertices[1].setColor(mOutlineColor);
		vertices[2].setColor(mOutlineColor);
		vertices[3].setColor(mOutlineColor);

		gl.setDrawMode(GL_LINE_LOOP);
		gl.draw_XYZ_RGBA(vertices, indices);
	}
}


TriangleShape::TriangleShape()
: mSize(0.0f)
{ }


void TriangleShape::setSize(float size)
{
	mSize = size;
}


void TriangleShape::draw()
{
	Vertex_Vector_XYZ_RGBA vertices;
	vertices.resize(3);
	float sqrt3 = sqrt(3.0f);

	// half side
	float a2 = mSize / sqrt3;

	// center from the bottom
	float x = mSize - ((2.0f * a2) / sqrt3);

	Vector2f edges[3];

	edges[0].x = -a2;
	edges[0].y = -x;
	edges[1].x = a2;
	edges[1].y = -x;
	edges[2].x = 0.0f;
	edges[2].y = mSize - x;

	for (int i = 0; i < 3; ++i)
	{
		mTransformation.transform(edges[i]);
		vertices[i].setPosition(edges[i]);
	}

	Index_Vector indices;
	indices.resize(3);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	
	GL_Render& gl = GL_Render::get();

	if (mFill)
	{
		vertices[0].setColor(mFillColor);
		vertices[1].setColor(mFillColor);
		vertices[2].setColor(mFillColor);

		gl.setDrawMode(GL_TRIANGLES);
		gl.draw_XYZ_RGBA(vertices, indices);
	}

	if (mOutline)
	{
		vertices[0].setColor(mOutlineColor);
		vertices[1].setColor(mOutlineColor);
		vertices[2].setColor(mOutlineColor);

		gl.setDrawMode(GL_LINE_LOOP);
		gl.draw_XYZ_RGBA(vertices, indices);
	}
}


void CircleShape::draw()
{
	Vertex_Vector_XYZ_RGBA vertices;
	vertices.resize(mNumSlices + 1);
	Index_Vector indices;
	indices.resize(mNumSlices + 1);

	float partAngle = 2.0f * NumericTraits<float>::pi() / mNumSlices;

	for (int i = 0; i < mNumSlices; ++i)
	{
		Vector2f p;
		p.x = cos(partAngle * static_cast<float>(i)) * mRadius;
		p.y = sin(partAngle * static_cast<float>(i)) * mRadius;
		mTransformation.transform(p);
		vertices[i].setPosition(p);
		indices[i] = i;
	}

	vertices[mNumSlices] = vertices[1];
	indices[mNumSlices] = 0;

	GL_Render& gl = GL_Render::get();

	if (mFill)
	{
		for (int i = 0; i < mNumSlices + 1; ++i)
		{
			vertices[i].setColor(mFillColor);
		}

		gl.setDrawMode(GL_TRIANGLE_FAN);
		gl.draw_XYZ_RGBA(vertices, indices);
	}

	if (mOutline)
	{
		for (int i = 0; i < mNumSlices + 1; ++i)
		{
			vertices[i].setColor(mOutlineColor);
		}

		gl.setDrawMode(GL_LINE_LOOP);
		gl.draw_XYZ_RGBA(vertices, indices);
	}
}
