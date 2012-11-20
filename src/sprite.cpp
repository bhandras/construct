#include "construct.h"


Sprite::Sprite()
: mAtlas(0)
, mCurrentKeyFrame(0)
, mTickMs(0)
, mFPS(0)
, mIsLooped(false)
, mColor(1.0f, 1.0f, 1.0f, 1.0f)
{ }


Sprite::~Sprite()
{
	for (size_t i = 0; i < mKeyFrames.size(); ++i)
	{
		delete mKeyFrames[i];
	}

	mKeyFrames.clear();
}


void Sprite::setAtlas(const TextureAtlas* atlas)
{
	mAtlas = atlas;
}


void Sprite::addKeyFrameImage(const std::string& name)
{
	if (mAtlas)
	{
		float x, y, w, h;
		if (mAtlas->getImage(name, x, y, w, h))
		{
			float textureW = static_cast<float>(mAtlas->getTexture().getW());
			float textureH = static_cast<float>(mAtlas->getTexture().getH());

			KeyFrame* keyFrame = new KeyFrame;
			keyFrame->quad.setSize(w, h);

			x /= textureW;
			y /= textureH;
			w /= textureW;
			h /= textureH;

			keyFrame->u0 = x;
			keyFrame->v0 = y;

			keyFrame->u1 = x + w;
			keyFrame->v1 = y;

			keyFrame->u2 = x + w;
			keyFrame->v2 = y + h;

			keyFrame->u3 = x;
			keyFrame->v3 = y + h;

			mKeyFrames.push_back(keyFrame);
		}
	}
}


void Sprite::setFPS(int FPS)
{
	mFPS = FPS;
}


void Sprite::setLooped(bool isLooped)
{
	mIsLooped = isLooped;
}


void Sprite::update(unsigned int deltaTimeMs)
{
	if (deltaTimeMs != 0 && mFPS != 0)
	{
		mTickMs += deltaTimeMs;
		int keyFrameDelta = mTickMs / (1000 / mFPS);

		if (keyFrameDelta != 0)
		{
			mTickMs = 0;
			if (mIsLooped)
			{
				mCurrentKeyFrame += keyFrameDelta;
				mCurrentKeyFrame %= mKeyFrames.size();
			}
			else
			{
				mCurrentKeyFrame = std::min(mCurrentKeyFrame + keyFrameDelta, mKeyFrames.size() - 1);
			}
		}
	}

	Actor2d::update(deltaTimeMs);
}


void Sprite::draw()
{
	KeyFrame* keyFrame = mKeyFrames[mCurrentKeyFrame];
	keyFrame->quad.setTransformation(mTransformation);
	keyFrame->quad.update();

	Vertex_Vector_XYZ_RGBA_UV vertices;
	vertices.resize(4);

	vertices[0].setPosition(keyFrame->quad.getEdge(Quad::QUAD_EDGE_TL));
	vertices[0].texture_uv[0] = keyFrame->u0;
	vertices[0].texture_uv[1] = keyFrame->v0;
	vertices[0].setColor(mColor);

	vertices[1].setPosition(keyFrame->quad.getEdge(Quad::QUAD_EDGE_TR));
	vertices[1].texture_uv[0] = keyFrame->u1;
	vertices[1].texture_uv[1] = keyFrame->v1;
	vertices[1].setColor(mColor);

	vertices[2].setPosition(keyFrame->quad.getEdge(Quad::QUAD_EDGE_BR));
	vertices[2].texture_uv[0] = keyFrame->u2;
	vertices[2].texture_uv[1] = keyFrame->v2;
	vertices[2].setColor(mColor);

	vertices[3].setPosition(keyFrame->quad.getEdge(Quad::QUAD_EDGE_BL));
	vertices[3].texture_uv[0] = keyFrame->u3;
	vertices[3].texture_uv[1] = keyFrame->v3;
	vertices[3].setColor(mColor);

	Index_Vector indices;
	indices.resize(6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	GL_Render& gl = GL_Render::get();
	gl.setDrawMode(GL_TRIANGLES);
	gl.setTexture(mAtlas->getTexture().getId());
	gl.enableBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gl.draw_XYZ_RGBA_UV(vertices, indices);
}
