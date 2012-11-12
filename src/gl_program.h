#pragma once


class GL_Program
{
public:
	enum ShaderType
	{
		VertexShader,
		FragmentShader
	};

	GL_Program();
	~GL_Program();

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
