#pragma once


#ifndef __APPLE__
	#define DBL_MAX 1.7976931348623158e+308	// max value
	#define DBL_MIN 2.2250738585072014e-308	// min positive value

	#define FLT_MAX 3.402823466e+38F		// max value
	#define FLT_MIN 1.175494351e-38F		// min positive value
#endif // __APPLE__


// Apple stuff required
#ifdef __APPLE__
	#include <Float.h>
#endif // __APPLE__


#define DBL_EPS 1e-12
#define FLT_EPS 1e-7f


namespace Construct
{
	template <class T>
	struct NumericTraits {};


	template<>
	struct NumericTraits<double>
	{
		static double eps()	{ return DBL_EPS; }
		static double min()	{ return -DBL_MAX; }
		static double max()	{ return DBL_MAX; }

		static double zero(void) { return 0.0; }
		static double one(void)	{ return 1.0; }
		static double pi(void) { return 3.14159265358979323846; }
		static double pi2(void)	{ return pi() * 2.0; }

		static int toInt(double a) { return (int)(a > 0 ? a + 0.5 : a - 0.5); }
	};


	template<>
	struct NumericTraits<float>
	{
		static float eps() { return FLT_EPS; }
		static float min() { return -FLT_MAX; }
		static float max() { return FLT_MAX; }

		static float zero() { return 0.0f; }
		static float one() { return 1.0f; }
		static float pi() { return 3.14159265358979323846f; }
		static float pi2() { return pi() * 2.f; }

		static int toInt(float a) { return (int)(a > 0 ? a + 0.5f : a - 0.5f ); }
	};
}
