#include "construct.h"

/*

2D OBB Intersection

Stefan Gottschalk's thesis (Collision Queries using Oriented Bounding
Boxes, Ph.D. Thesis, Department of Computer Science, University of North
Carolina at Chapel Hill, 1999) introduces the separating-axis method
for performing the equivalent test on 3D oriented bounding boxes.
This method depends on the observation that for two boxes to be disjoint
(i.e. *not* intersecting), there must be some axis along which their projections
are disjoint. The 3D case considers each of 15 axes as a potential
separating axis. These axes are the three edge axes of box 1, the three edge
axes of box 2, and the nine cross products formed by taking some edge of box 1
and some edge of box 2.  

In 2D this simplifies dramatically and only four axes need be considered. 
These are the orthogonal edges of each bounding box. If a few values are precomputed
every time a box moves, we end up performing only 16 dot products and some
comparisons in the worst case for each overlap test. One nice property of the
separating-axis method is that it can be structured in an early out fashion, 
so that many fewer operations are needed in the case where the boxes do not intersect.
In general, the first test is extremely likely to fail (and return "no overlap")
when there is no overlap.  If it passes, the second test is even more likely
to fail if there is no overlap, and so on. Only when the boxes are in extremely
close proximity is there even a 50% chance of executing more than 2 tests.

*/


Quad::Quad()
: mHotSpot(QUAD_HOTSPOT_TL)
{ }


Quad::Quad(const Quad& other)
{
	mSize = other.mSize;
	mHotSpot = other.mHotSpot;

	mCorners[0] = other.mCorners[0];
	mCorners[1] = other.mCorners[1];
	mCorners[2] = other.mCorners[2];
	mCorners[3] = other.mCorners[3];

	mAxis[0] = other.mAxis[0];
	mAxis[1] = other.mAxis[1];

	mOrigin[0] = other.mOrigin[0];
	mOrigin[1] = other.mOrigin[1];
}


Quad& Quad::operator=(const Quad& other)
{
	if (this != &other)
	{
		mSize = other.mSize;
		mHotSpot = other.mHotSpot;

		mCorners[0] = other.mCorners[0];
		mCorners[1] = other.mCorners[1];
		mCorners[2] = other.mCorners[2];
		mCorners[3] = other.mCorners[3];

		mAxis[0] = other.mAxis[0];
		mAxis[1] = other.mAxis[1];

		mOrigin[0] = other.mOrigin[0];
		mOrigin[1] = other.mOrigin[1];
	}

	return *this;
}


void Quad::update(unsigned int deltaTimeMs)
{
	// create an axis aligned box first
	Vector2f x(mSize.w, 0.0f);
	Vector2f y(0.0f, mSize.h);

	mCorners[QUAD_CORNER_TL].x = 0.0f;
	mCorners[QUAD_CORNER_TL].y = 0.0f;
	mCorners[QUAD_CORNER_TR] = x;
	mCorners[QUAD_CORNER_BR] = x + y;
	mCorners[QUAD_CORNER_BL] = y;
	mCentroid = (mCorners[0] + mCorners[1] + mCorners[2] + mCorners[3]) * 0.25f;

	// calculate hot spot delta vector
	Vector2f hotSpotDelta;
	switch (mHotSpot)
	{
	case QUAD_HOTSPOT_CENTER:
		hotSpotDelta = mCorners[QUAD_CORNER_TL] - mCentroid;
		break;
	case QUAD_HOTSPOT_TR:
		hotSpotDelta = mCorners[QUAD_CORNER_TL] - mCorners[QUAD_CORNER_TR];
		break;
	case QUAD_HOTSPOT_BL:
		hotSpotDelta = mCorners[QUAD_CORNER_TL] - mCorners[QUAD_CORNER_BL];
		break;
	case QUAD_HOTSPOT_BR:
		hotSpotDelta = mCorners[QUAD_CORNER_TL] - mCorners[QUAD_CORNER_BR];
		break;
	case QUAD_HOTSPOT_TL:
	default:
		hotSpotDelta = Vector2f(0, 0);
		break;
	}

	mCentroid += hotSpotDelta;
	mTransformation.transform(mCentroid);
	
	// create corners
	for (int i = 0; i < 4; ++i)
	{
		mCorners[i] += hotSpotDelta;
		mTransformation.transform(mCorners[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		mNormals[i] = mCorners[(i+1) % 4] - mCorners[i];
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


void Quad::setHotSpot(HotSpot hotSpot)
{
	mHotSpot = hotSpot;
}


void Quad::setTransformation(const Affine2df& t)
{
	mTransformation = t;
}


bool Quad::isInside(const Vector2f& point) const
{
	float angle = 0.0f;

	for (int i = 0; i < 4; ++i)
	{
		Vector2f p1(mCorners[i] - point);
		Vector2f p2(mCorners[(i + 1) % 4] - point);
		angle += p1.angle(p2);
	}

	if (fabs(angle) < M_PI)
	{
		return false;
	}
	
	return true;
}


bool Quad::overlaps(const Quad& other) const
{
	return overlaps1Way(other) && other.overlaps1Way(*this);
}


void Quad::intersect(const Vector2f& p0, const Vector2f& p1, Vector2f& result)
{
	Vector2f v = p1 - p0;
	v.normalize();

	for (int i = 0; i < 4; i += 1)
	{
		int j = (i + 1) % 4;
		float dot = v.dot(mNormals[i]);
		if (dot < 0.0f && Intersection2d::line(mCorners[i], mCorners[j], p0, p1, result))
		{
			return;
		}
	}

	result = p1;
}


void Quad::draw(const Color4& color, float x0, float y0, float x1, float y1) const
{
	Vertex_Vector_XYZ_RGBA vertices;
	vertices.resize(8);

	Index_Vector indices;
	indices.resize(8);

	Vector2f v(x1 - x0, y1 - y0);
	v.normalize();
	GL_Render& gl = GL_Render::get();

	for (int i = 0; i < 8; i += 2)
	{
		Vector2f a = mCorners[(i/2) % 4];
		Vector2f b = mCorners[(i/2 + 1) % 4];
		Vector2f t = a + (b - a) * 0.5f;
		Vector2f n = mNormals[(i/2)%4];

		// draw normal
		Vertex_Vector_XYZ_RGBA line;
		line.resize(2);
		line[0].setPosition(t);
		line[0].setColor(1.0f, 1.0f, 1.0f, 1.0f);
		line[1].setPosition(t + (n * 20.0f));
		line[1].setColor(1.0f, 1.0f, 1.0f, 1.0f);

		Index_Vector ind;
		ind.resize(2);
		ind[0] = 0;
		ind[1] = 1;
		gl.setDrawMode(GL_LINES);
		gl.draw_XYZ_RGBA(line, ind);

		vertices[i].setPosition(mCorners[(i/2) % 4]);
		vertices[(i + 1)%8].setPosition(mCorners[(i/2 + 1)%4]);

		Vector2f intersection;
		float dot = v.dot(mNormals[(i/2)%4]);
		if (dot < 0.0f)
		{
			vertices[i].setColor(1.0f, 0.0f, 0.0f, 1.0f);
			vertices[(i+1)%8].setColor(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			vertices[i].setColor(color);
			vertices[(i + 1)%8].setColor(color);
		}
	}
	

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;
	indices[6] = 6;
	indices[7] = 7;

	
	gl.setDrawMode(GL_LINES);
	gl.draw_XYZ_RGBA(vertices, indices);
}


void Quad::refresh()
{
	mAxis[0] = mCorners[1] - mCorners[0]; 
	mAxis[1] = mCorners[3] - mCorners[0]; 

	// Make the length of each axis (1 / edge) length so we know
	// any dot product must be less than 1 to fall within the edge.
	for (int i = 0; i < 2; ++i)
	{
		mAxis[i] *= (1.0f / mAxis[i].squareLength());
		mOrigin[i] = mCorners[i].dot(mAxis[i]);
	}
}


bool Quad::overlaps1Way(const Quad& other) const
{
	for (int a = 0; a < 2; ++a)
	{
		float t = other.mCorners[0].dot(mAxis[a]);

		// Find the extent of box 2 on axis a
		double tMin = t;
		double tMax = t;

		for (int c = 1; c < 4; ++c)
		{
			t = other.mCorners[c].dot(mAxis[a]);

			if (t < tMin)
			{
				tMin = t;
			}
			else if (t > tMax)
			{
				tMax = t;
			}
		}

		// We have to subtract off the origin

		// See if [tMin, tMax] intersects [0, 1]
		if ((tMin > 1 + mOrigin[a]) || (tMax < mOrigin[a]))
		{
			// There was no intersection along this dimension;
			// the boxes cannot possibly overlap.
			return false;
		}
	}

	// There was no dimension along which there is no intersection.
	// Therefore the boxes overlap.
	return true;
}
