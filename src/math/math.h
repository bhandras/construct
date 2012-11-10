#pragma once


// math
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#ifndef M_PI_4
#define M_PI_4 0.785398163397448309616
#endif

#include "numeric_traits.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

#include "matrix44.h"
#include "matrix44_stack.h"

#include "affine_2d.h"


typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;

typedef Matrix44<float> Matrix44f;
typedef Matrix44<double> Matrix44d;

typedef Matrix44Stack<float> Matrix44fStack;
typedef Matrix44Stack<double> Matrix44dStack;

typedef Affine2d<float> Affine2df;

#include "math_util.h"
