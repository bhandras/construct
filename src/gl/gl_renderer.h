#pragma once


#include <vector>
#include <list>
#include <map>

#include "../math/math.h"


struct Vertex_XYZ_RGBA
{
	Vertex_XYZ_RGBA()
	{
		position[0] = position[1] = position[2] = 0.0f;
		color_rgba[0] = color_rgba[1] = color_rgba[2] = color_rgba[3] = 0.0f;
	}

	void setPosition(const Vector2f& v)
	{
		position[0] = v.x();
		position[1] = v.y();
		position[2] = 0.0f;
	}

	void setColor(float r, float g, float b, float a)
	{
		color_rgba[0] = r;
		color_rgba[1] = g;
		color_rgba[2] = b;
		color_rgba[3] = a;
	}

	GLfloat position[3];
	GLfloat color_rgba[4];
};


struct Vertex_XYZ_RGBA_UV
{
	Vertex_XYZ_RGBA_UV()
	{
		position[0] = position[1] = position[2] = 0.0f;
		color_rgba[0] = color_rgba[1] = color_rgba[2] = color_rgba[3] = 0.0f;
		texture_uv[0] = texture_uv[1] = 0.0f;
	}

	void setPosition(const Vector2f& v)
	{
		position[0] = v.x();
		position[1] = v.y();
		position[2] = 0.0f;
	}

	void setColor(float r, float g, float b, float a)
	{
		color_rgba[0] = r;
		color_rgba[1] = g;
		color_rgba[2] = b;
		color_rgba[3] = a;
	}

	GLfloat position[3];
	GLfloat color_rgba[4];
	GLfloat texture_uv[2];
};


typedef std::vector<GLshort> Index_Vector;
typedef std::vector<Vertex_XYZ_RGBA> Vertex_Vector_XYZ_RGBA;
typedef std::vector<Vertex_XYZ_RGBA_UV> Vertex_Vector_XYZ_RGBA_UV;


class GLRenderer
{
public:
	enum DrawMode
	{
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		TRIANGLES,
		LINE_STRIP,
		LINES
	};

	static GLRenderer* getInstance()
	{
		if (!mInstance)
		{
			mInstance = new GLRenderer;
		}

		return mInstance;
	}

	void init();

	GLProgram* getActiveProgram() { return mActiveProgram; }

	void setActiveProgram(const char* name);
	void attachShader(const char* program, const char* path, GLProgram::ShaderType type);
	void setOrthoProjection(float left, float right, float bottom, float top, float near, float far);

	void draw(DrawMode mode, const Vertex_Vector_XYZ_RGBA& vertices, const Index_Vector& indices);
	void draw(DrawMode mode, const Vertex_Vector_XYZ_RGBA_UV& vertices, const Index_Vector& indices, GLuint textureId);
	void flush();

private:
	GLRenderer();
	~GLRenderer();

	// do not copy
	GLRenderer(const GLRenderer&);
	GLRenderer operator=(const GLRenderer&);

	static GLRenderer* mInstance;

	typedef std::map<std::string, GLProgram> ProgramMap;
	ProgramMap mPrograms;
	GLProgram* mActiveProgram;

	// todo: style
	struct XYZ_RGBA_Batch
	{
		Vertex_Vector_XYZ_RGBA vertices;
		Index_Vector indices;
	};

	struct XYZ_RGBA_UV_Batch
	{
		GLuint textureId;
		Vertex_Vector_XYZ_RGBA_UV vertices;
		Index_Vector indices;
	};

	typedef std::map<DrawMode, XYZ_RGBA_Batch> XYZ_RGBA_Batch_Map;
	typedef std::map< DrawMode, std::list<XYZ_RGBA_UV_Batch> > XYZ_RGBA_UV_Batch_Map;

	XYZ_RGBA_Batch_Map mVertex_Batches_XYZ_RGBA;
	XYZ_RGBA_UV_Batch_Map mVertex_Batches_XYZ_RGBA_UV;

	Matrix44f mProjection;
};
