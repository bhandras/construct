#pragma once


#ifdef __APPLE__

#include <Availability.h>

// GLES2 includes
#ifdef __MAC_OS_X_VERSION_MAX_ALLOWED
	#include <OpenGL/OpenGL.h>
	#include <GLUT/GLUT.h>
#else
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
#endif

#endif //__APPLE__

#ifdef _WIN32
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

	#include <windows.h>
	#include "GLES2/gl2.h"
	#include "EGL/egl.h"
#endif // _WIN32



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
#include "functor.h"
#include "xml.h"
#include "color4.h"
#include "input.h"

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

#include "math/numeric_traits.h"
#include "math/vector2.h"
#include "math/vector3.h"
#include "math/matrix44.h"
#include "math/affine2d.h"
#include "math/math_util.h"


// opengl es 2.0
#include "gl_shaders.h"
#include "gl_program.h"
#include "gl_renderer.h"
#include "gl_texturemanager.h"


// physics
#include "physics/gjk.h"
#include "physics/collision.h"
#include "physics/shape.h"
#include "physics/body.h"
#include "physics/polygon.h"
#include "physics/contact.h"
#include "physics/space.h"


// 2d vis
#include "context.h"
#include "texture_atlas.h"
#include "actor2d.h"
#include "quad.h"
#include "basic_shapes.h"
#include "sprite.h"
#include "bitmap_font.h"

