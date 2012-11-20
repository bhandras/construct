#include "construct.h"


Triangle::Triangle()
: mSize(0.0f)
{
	mEdges.resize(3);
	mNormals.resize(3);
}


void Triangle::setSize(float size)
{
	mSize = size;
}


void Triangle::update()
{
	float sqrt3 = sqrt(3.0f);

	// half side
	float a2 = mSize / sqrt3;

	// center from the bottom
	float x = mSize - ((2.0f * a2) / sqrt3);

	mEdges[0].x = -a2;
	mEdges[0].y = -x;
	mEdges[1].x = a2;
	mEdges[1].y = -x;
	mEdges[2].x = 0.0f;
	mEdges[2].y = mSize - x;

	for (size_t i = 0; i < 3; ++i)
	{
		mTransformation.transform(mEdges[i]);
	}
}


Circle::Circle(int numSlices)
: mRadius(0.0f)
{
	mEdges.resize(numSlices);
	mNormals.resize(numSlices);
}


void Circle::update()
{
	int numSlices = mEdges.size();
	float partAngle = 2.0f * NumericTraits<float>::pi() / numSlices;

	for (int i = 0; i < numSlices; ++i)
	{
		mEdges[i].x = cos(partAngle * static_cast<float>(i)) * mRadius;
		mEdges[i].y = sin(partAngle * static_cast<float>(i)) * mRadius;
		mTransformation.transform(mEdges[i]);
	}
}
