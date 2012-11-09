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

#include "gl_program.h"
#include "gl_renderer.h"
#include "gl_texturemanager.h"
