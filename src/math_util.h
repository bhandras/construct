#pragma once


namespace Construct
{
	namespace MathUtil
	{
		class Random
		{
		public:
			static void setSeed(int newSeed);
			static int getInteger(int a, int b);
			static float getFloat(float a, float b);

		private:
			static int seed;
		};


		class PerlinNoise
		{
		public:
			static float perlinNoise1d(float x);
			static float perlinNoise2d(float x, float y);
		};

		class Color
		{
		public:
			// r, g, b = [0, 1]
			// h = [0,360], s, v = [0,1]
			// if s == 0, then h = -1 (undefined)
			static void rgb2hsv(float r, float g, float b, float& h, float& s, float& v);
			static void hsv2rgb(float h, float s, float v, float& r, float& g, float& b);
		};

		class Numeric
		{
		public:
			static int toInt(float num);
			static int toInt(double num);
			static float deg2Rad(float angleDeg);
			static float rad2Deg(float angleRad);
		};
	} // MathUtil
}
