#include "construct.h"


namespace Construct
{

	GL_Program::GL_Program()
		: mProgramId(0)
	{ }


	GL_Program::~GL_Program()
	{
		destroy();
	}


	void GL_Program::setShader(GL_Program::ShaderType type, const std::string& source)
	{
		const char* header[2] = {
			// Define GLSL version
#ifdef GL_ES_VERSION_2_0
			"#version 100\n"
#else
			"#version 120\n"
#endif
			,
			// GLES2 precision specifiers
#ifdef GL_ES_VERSION_2_0
			// Define default float precision for fragment shaders:
			(type == GL_Program::FragmentShader) ?
			"precision mediump float;\n"
			: ""
			// Note: OpenGL ES automatically defines this:
			// #define GL_ES
#else
			// Ignore GLES 2 precision specifiers:
			"#define lowp\n"
			"#define mediump\n"
			"#define highp\n"
#endif
		};

		mShaderSources[type] = std::string(header[0]) + std::string(header[1]) + source;
	}


	bool GL_Program::isLinked()
	{
		return mProgramId != 0;
	}


	bool GL_Program::link()
	{
		if (!create())
		{
			return false;
		}

		if (!attachShader(GL_VERTEX_SHADER, mShaderSources[VertexShader]))
		{
			destroy();
			return false;
		}

		if (!attachShader(GL_FRAGMENT_SHADER, mShaderSources[FragmentShader]))
		{
			destroy();
			return false;
		}

		glLinkProgram(mProgramId);
		GLint statusCode;
		glGetProgramiv(mProgramId, GL_LINK_STATUS, &statusCode);

		if (statusCode != GL_TRUE)
		{
			int logLen = 0;
			glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &logLen);
			if (logLen > 0)
			{
				char* log = new char[logLen];
				glGetProgramInfoLog(mProgramId, logLen, &logLen, log);
				std::cerr << "GL program link failed: " << log << std::endl;
				delete [] log;
			}
			else
			{
				std::cerr << "GL program link failed." << std::endl;
			}

			return false;
		}

		return true;
	}


	void GL_Program::use()
	{
		glUseProgram(mProgramId);
	}


	GLuint GL_Program::getAttributeLocation(const char* attributeName) const
	{
		GLint location = glGetAttribLocation(mProgramId, attributeName);
		if (location == -1)
		{
			std::cerr << "Attribute \"" << attributeName << "\" not found" << std::endl;
		}
		return location;
	}


	GLuint GL_Program::getUniformLocation(const char* uniformName) const
	{
		GLint location = glGetUniformLocation(mProgramId, uniformName);
		if (location == -1)
		{
			std::cerr << "Uniform \"" << uniformName << "\" not found" << std::endl;
		}
		return location;
	}


	bool GL_Program::create()
	{
		if (mProgramId == 0)
		{
			mProgramId = glCreateProgram();
		}
		return mProgramId != 0;
	}


	void GL_Program::destroy()
	{
		if (mProgramId != 0)
		{
			glDeleteProgram(mProgramId);
			mProgramId = 0;
		}
	}


	bool GL_Program::attachShader(GLenum shaderType, const std::string& source)
	{
		GLuint shaderId = glCreateShader(shaderType);
		GLint length = static_cast<GLint>(source.length());
		const char* sourcePtr = source.c_str();
		glShaderSource(shaderId, 1, &sourcePtr, &length);
		glCompileShader(shaderId);

		GLint compileStatus = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

		if (compileStatus != GL_TRUE)
		{
			std::cerr << "Shader compilation failed! Shader type: " << (shaderType == GL_VERTEX_SHADER ? "vertex" : (shaderType == GL_FRAGMENT_SHADER ? "fragment" : "bad type")) << " ";
			int logLength = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0)
			{
				char* log = new char[logLength];
				glGetShaderInfoLog(shaderId, logLength, &logLength, log);
				std::cerr << "info: " << log << std::endl;
				delete [] log;
			}
			else
			{
				std::cerr << "(no info)" << std::endl;
			}

			glDeleteShader(shaderId);
			return false;
		}

		glAttachShader(mProgramId, shaderId);
		glDeleteShader(shaderId);

		return true;
	}
}
