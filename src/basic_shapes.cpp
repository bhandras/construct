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

	if (mDrawArea)
	{
		vertices[0].setColor(mAreaFillColor);
		vertices[1].setColor(mAreaFillColor);
		vertices[2].setColor(mAreaFillColor);
		vertices[3].setColor(mAreaFillColor);

		gl.setDrawMode(GL_TRIANGLES);
		gl.draw_XYZ_RGBA(vertices, indices);
	}

	if (mDrawOutline)
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

	if (mDrawArea)
	{
		vertices[0].setColor(mAreaFillColor);
		vertices[1].setColor(mAreaFillColor);
		vertices[2].setColor(mAreaFillColor);

		gl.setDrawMode(GL_TRIANGLES);
		gl.draw_XYZ_RGBA(vertices, indices);
	}

	if (mDrawOutline)
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
#pragma message("todo")
	/*
	float vertices[34];

	for (int i = 0; i < 32; i += 2)
	{
		vertices[i] = mX + cos(MathUtil::deg2Rad(360.0f / 32.0f * static_cast<float>(i))) * mRadius;
		vertices[i+1] = mY + sin(MathUtil::deg2Rad(360.0f / 32.0f * static_cast<float>(i))) * mRadius;
	}
	vertices[32] = vertices[0];
	vertices[33] = vertices[1];

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);

	if (mDrawArea)
	{
		mAreaFillColor.glColor();
		glDrawArrays(GL_TRIANGLE_FAN, 0, 16);
	}

	if (mDrawOutline)
	{
		mOutlineColor.glColor();
		glDrawArrays(GL_LINE_STRIP, 0, 17);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	*/
}
