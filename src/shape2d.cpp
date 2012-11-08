#include "math/math.h"
#include "gl/gl.h"

#include "shape2d.h"


Shape2d::Shape2d()
: mDirty(true)
, mPosX(0.0f)
, mPosY(0.0f)
, mRotationDeg(0.0f)
, mScale(1.0f)
, mAreaFillColor(1.0f, 1.0f, 1.0f, 1.0f)
, mOutlineColor(1.0f, 1.0f, 1.0f, 1.0f)
, mDrawOutline(false)
{ }


void Shape2d::setPosX(int x)
{
	mPosX = static_cast<float>(x);
	mDirty = true;
}


void Shape2d::setPosX(float x)
{
	mPosX = x;
	mDirty = true;
}


void Shape2d::setPosY(int y)
{
	mPosY = static_cast<float>(y);
	mDirty = true;
}


void Shape2d::setPosY(float y)
{
	mPosY = y;
	mDirty = true;
}


void Shape2d::setPosition(int x, int y)
{
	mPosX = static_cast<float>(x);
	mPosY = static_cast<float>(y);
	mDirty = true;
}


void Shape2d::setPosition(float x, float y)
{
	mPosX = x;
	mPosY = y;
	mDirty = true;
}


void Shape2d::setRotation(float rotationDeg)
{
	mRotationDeg = rotationDeg;
	mDirty = true;
}


void Shape2d::setScale(float scale)
{
	mScale = scale;
	mDirty = true;
}


void RectangleShape::createShade(float* colorArray, int& arraySize)
{
	float vec[7][2] = {
		{0.195f, 0.02f}, {0.383f, 0.067f},
		{0.55f, 0.169f}, {0.707f, 0.293f},
		{0.831f, 0.45f}, {0.924f, 0.617f},
		{0.98f, 0.805f}
	};

	for (int i = 0; i < 7; ++i)
	{
		vec[i][0] *= mCornerRadius;
		vec[i][1] *= mCornerRadius;
	}

	Color4 colorTop = mAreaFillColor + Color4(mShadeMax, mShadeMax, mShadeMax, 0.0f);
	Color4 colorBottom = mAreaFillColor + Color4(mShadeMin, mShadeMin, mShadeMin, 0.0f);

	colorTop.normalize();
	colorBottom.normalize();
	arraySize = 0;

	// color
	if (mCornerType & CORNER_ROUND_BR)
	{
		colorBottom.get(&colorArray[arraySize]);
		arraySize += 4;

		for (int a = 0; a < 7; ++a)
		{
			Color4 c(colorTop.interpolate(colorBottom, vec[a][1] / mH));
			c.get(&colorArray[arraySize]);
			arraySize += 4;
		}

		Color4 c(colorTop.interpolate(colorBottom, mCornerRadius / mH));
		c.get(&colorArray[arraySize]);
		arraySize += 4;
	}
	else
	{
		colorBottom.get(&colorArray[arraySize]);
		arraySize += 4;
	}

	if (mCornerType & CORNER_ROUND_TR)
	{
		Color4 c(colorTop.interpolate(colorBottom,  (mH - mCornerRadius) / mH));
		c.get(&colorArray[arraySize]);
		arraySize += 4;
		
		for (int a = 0; a < 7; ++a)
		{
			Color4 c(colorTop.interpolate(colorBottom, (mH - mCornerRadius + vec[a][1]) / mH));
			c.get(&colorArray[arraySize]);
			arraySize += 4;
		}

		colorTop.get(&colorArray[arraySize]);
		arraySize += 4;
	}
	else
	{
		colorTop.get(&colorArray[arraySize]);
		arraySize += 4;
	}


	if (mCornerType & CORNER_ROUND_TL)
	{
		colorTop.get(&colorArray[arraySize]);
		arraySize += 4;

		for (int a = 0; a < 7; ++a)
		{
			Color4 c(colorTop.interpolate(colorBottom, (mH - vec[a][1]) / mH));
			c.get(&colorArray[arraySize]);
			arraySize += 4;
		}

		Color4 c(colorTop.interpolate(colorBottom, (mH - mCornerRadius) / mH));
		c.get(&colorArray[arraySize]);
		arraySize += 4;
	}
	else
	{
		colorTop.get(&colorArray[arraySize]);
		arraySize += 4;
	}

	if (mCornerType & CORNER_ROUND_BL)
	{
		Color4 c(colorTop.interpolate(colorBottom, mCornerRadius / mH));
		c.get(&colorArray[arraySize]);
		arraySize += 4;

		for (int a = 0; a < 7; ++a)
		{
			Color4 c(colorTop.interpolate(colorBottom, (mCornerRadius - vec[a][1]) / mH));
			c.get(&colorArray[arraySize]);
			arraySize += 4;
		}

		colorBottom.get(&colorArray[arraySize]);
		arraySize += 4;
	}
	else
	{
		colorBottom.get(&colorArray[arraySize]);
		arraySize += 4;
	}
}


void RectangleShape::draw()
{
	static const int maxVertices = 4 * (7 + 2);

	float vec[7][2] = {
		{0.195f, 0.02f}, {0.383f, 0.067f},
		{0.55f, 0.169f}, {0.707f, 0.293f},
		{0.831f, 0.45f}, {0.924f, 0.617f},
		{0.98f, 0.805f}
	};

	for (int a = 0; a < 7; ++a)
	{
		vec[a][0] *= mCornerRadius;
		vec[a][1] *= mCornerRadius;
	}

	float vertices[maxVertices * 2];
	float outline[maxVertices * 2];
	int i = 0;
	int j = 0;

	if (mCornerType & CORNER_ROUND_BR)
	{
		vertices[i++] = mW - mCornerRadius;
		vertices[i++] = 0.0f;

		outline[j++] = mW - mCornerRadius;
		outline[j++] = -0.5f;

		for (int a = 0; a < 7; ++a)
		{
			vertices[i++] = mW - mCornerRadius + vec[a][0];
			vertices[i++] = vec[a][1];

			outline[j++] = mW - mCornerRadius + vec[a][0];
			outline[j++] = vec[a][1] - 0.5f;
		}

		vertices[i++] = mW;
		vertices[i++] = mCornerRadius;

		outline[j++] = mW + 0.5f;
		outline[j++] = mCornerRadius;
	}
	else
	{
		vertices[i++] = mW;
		vertices[i++] = 0.0f;

		outline[j++] = mW + 0.5f;
		outline[j++] = -0.5f;
	}

	if (mCornerType & CORNER_ROUND_TR)
	{
		vertices[i++] = mW;
		vertices[i++] = mH - mCornerRadius;

		outline[j++]= mW + 0.5f;
		outline[j++] = mH - mCornerRadius;

		for (int a = 0; a < 7; ++a)
		{
			vertices[i++] = mW - vec[a][1];
			vertices[i++] = mH - mCornerRadius + vec[a][0];

			outline[j++] = mW + 0.5f - vec[a][1];
			outline[j++] = mH - mCornerRadius + vec[a][0];
		}

		vertices[i++] = mW - mCornerRadius;
		vertices[i++] = mH;

		outline[j++] = mW - mCornerRadius;
		outline[j++]= mH + 0.5f;
	}
	else
	{
		vertices[i++] = mW;
		vertices[i++] = mH;

		outline[j++] = mW + 0.5f;
		outline[j++] = mH + 0.5f;
	}

	if (mCornerType & CORNER_ROUND_TL)
	{
		vertices[i++] = mCornerRadius;
		vertices[i++] = mH;

		outline[j++] = mCornerRadius;
		outline[j++] = mH + 0.5f;

		for (int a = 0; a < 7; ++a)
		{

			vertices[i++] = mCornerRadius - vec[a][0];
			vertices[i++] = mH - vec[a][1];

			outline[j++] = mCornerRadius - vec[a][0];
			outline[j++] = mH + 0.5f - vec[a][1];
		}

		vertices[i++] = 0.0f;
		vertices[i++] = mH - mCornerRadius;

		outline[j++] = -0.5f;
		outline[j++] = mH - mCornerRadius;
	}
	else
	{
		vertices[i++] = 0.0f;
		vertices[i++] = mH;

		outline[j++] = -0.5f;
		outline[j++] = mH + 0.5f;
	}

	if (mCornerType & CORNER_ROUND_BL)
	{
		vertices[i++] = 0.0f;
		vertices[i++] = mCornerRadius;

		outline[j++] = -0.5f;
		outline[j++] = mCornerRadius;

		for (int a = 0; a < 7; ++a)
		{
			vertices[i++] = vec[a][1];
			vertices[i++] = mCornerRadius - vec[a][0];

			outline[j++] = vec[a][1] - 0.5f;
			outline[j++] = mCornerRadius - vec[a][0];
		}

		vertices[i++] = mCornerRadius;
		vertices[i++] = 0.0f;

		outline[j++] = mCornerRadius;
		outline[j++] = -0.5f;
	}
	else
	{
		vertices[i++] = 0.0f;
		vertices[i++] = 0.0f;

		outline[j++] = -0.5f;
		outline[j++] = -0.5f;
	}
#pragma message("todo")
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &vertices[0]);

	float sizeX2 = mW / 2.0f;
	float sizeY2 = mH / 2.0f;

	glPushMatrix();
	glTranslatef(static_cast<float>(mX + sizeX2), static_cast<float>(mY + sizeY2), 0.0f);
	glRotatef(mRotationAngle, 0.0f, 0.0f, 1.0f);
	glTranslatef(static_cast<float>(-sizeX2),  static_cast<float>(-sizeY2), 0.0f);

	glScalef(mScale, mScale, 0.0f);
	glDisable(GL_TEXTURE_2D);
*/

	/*
	if (mDrawArea)
	{
		float colorArray[4 * maxVertices];
		if (mIsShaded)
		{
			int colorArraySize = 0;
			createShade(colorArray, colorArraySize);
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, 0, colorArray);
			glShadeModel(GL_SMOOTH);
		}

		mAreaFillColor.glColor();
		glDrawArrays(GL_TRIANGLE_FAN, 0, i / 2);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	if (mDrawOutline)
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);

		glVertexPointer(2, GL_FLOAT, 0, &outline[0]);
		mOutlineColor.glColor();
		glDrawArrays(GL_LINE_LOOP, 0, j / 2);

		glDisable(GL_LINE_SMOOTH);
	}

	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
	*/
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


TriangleShape::TriangleShape()
: mSize(0.0f)
{
	mVertices.resize(3);
}


void TriangleShape::setSize(float size)
{
	mSize = size;
	mDirty = true;
}


void TriangleShape::draw()
{
	if (mDirty)
	{
		// half side
		float a2 = mSize / sqrt(3.0f);
		
		// half size
		float size2 = mSize / 2.0f;
		
		Vector2f A(mPosX - a2, mPosY - size2);
		Vector2f B(mPosX + a2, mPosY - size2);
		Vector2f C(mPosX, mPosY + size2);

		mVertices[0].setPosition(A);
		mVertices[1].setPosition(B);
		mVertices[2].setPosition(C);
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
