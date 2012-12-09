#pragma once


namespace Construct
{
	class Actor2d
	{
	public:
		Actor2d();
		virtual ~Actor2d() {}

		void setPosition(float x, float y);
		void getPosition(float& x, float& y);

		void setRotationDeg(float angleDeg);
		void setScale(float scaleX, float scaleY);

		void update(unsigned int deltaTimeMs);

	protected:
		bool mDirty;

		float mPosX;
		float mPosY;
		float mRotationRad;
		float mScaleX;
		float mScaleY;

		Affine2df mTransformation;
	};
}
