#pragma once


#include <string>


class GLProgram
{
public:
	enum ShaderType
	{
		VertexShader,
		FragmentShader
	};

	GLProgram();
	~GLProgram();

	void setShader(ShaderType type, const std::string& source);

	bool isLinked();
	bool link();

	void use();

	GLuint getAttributeLocation(const char* attributeName) const;
	GLuint getUniformLocation(const char* uniformName) const;

private:
	bool create();
	void destroy();
	bool attachShader(GLenum shaderType, const std::string& source);

private:
	GLuint mProgramId;
	std::string mShaderSources[2];
};
