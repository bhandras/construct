#pragma once

// GLES2 includes
#if defined(TARGET_OS_IPHONE)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#endif

#define MAX_OPENGL_TEXTURE_MEMORY 64 * 1024 * 1024

// std lib
#include <string>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>


// common stuff
#include "foreach.h"
#include "xml.h"
#include "color4.h"


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
#include "matrix44.h"
#include "matrix44_stack.h"
#include "affine_2d.h"
#include "math_util.h"

typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Matrix44<float> Matrix44f;
typedef Matrix44Stack<float> Matrix44fStack;
typedef Affine2d<float> Affine2df;

// opengl es 2.0
#include "gl_shaders.h"
#include "gl_program.h"
#include "gl_renderer.h"
#include "gl_texturemanager.h"

// 2d vis
#include "texture_atlas.h"
#include "quad.h"
#include "actor2d.h"
#include "shape2d.h"
#include "basic_shapes.h"
#include "spirte.h"
#include "bitmap_font.h"
