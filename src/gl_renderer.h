#pragma once


struct Vertex_XYZ_RGBA
{
	Vertex_XYZ_RGBA()
	{
		position[0] = position[1] = position[2] = 0.0f;
		color_rgba[0] = color_rgba[1] = color_rgba[2] = color_rgba[3] = 0.0f;
	}

	void setPosition(const Vector2f& v)
	{
		position[0] = v.x;
		position[1] = v.y;
		position[2] = 0.0f;
	}

	void setColor(const Color4& color)
	{
		color.get(color_rgba);
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
		position[0] = v.x;
		position[1] = v.y;
		position[2] = 0.0f;
	}

	void setColor(const Color4& color)
	{
		color.get(color_rgba);
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


struct RenderBatch
{
	enum Type
	{
		T_XYZ_RGBA,
		T_XYZ_RGBA_UV
	};

	Type type;

	virtual ~RenderBatch() {}
};


struct XYZ_RGBA_RenderBatch : public RenderBatch
{
	XYZ_RGBA_RenderBatch()
	{
		type = RenderBatch::T_XYZ_RGBA;
	}

	Vertex_Vector_XYZ_RGBA vertices;
	Index_Vector indices;
};


struct XYZ_RGBA_UV_RenderBatch : public RenderBatch
{
	XYZ_RGBA_UV_RenderBatch()
	{
		type = RenderBatch::T_XYZ_RGBA_UV;
	}

	GLuint textureId;
	Vertex_Vector_XYZ_RGBA_UV vertices;
	Index_Vector indices;
};


class GL_Render
{
public:
	static GL_Render& get()
	{
		static GL_Render instance;
		return instance;
	}

	void init();

	void beginFrame();
	void endFrame();

	void setDrawMode(GLenum drawMode);
	void setTexture(GLuint textureId);
	void enableBlending(GLenum sFactor, GLenum dFactor);
	void disableBlending();

	void setOrthoProjection(float left, float right, float bottom, float top, float near, float far);

	void draw_XYZ_RGBA(const Vertex_Vector_XYZ_RGBA& vertices, const Index_Vector& indices);
	void draw_XYZ_RGBA_UV(const Vertex_Vector_XYZ_RGBA_UV& vertices, const Index_Vector& indices);

	void flush();

	// statistics
	int getNumDrawCalls() const;

private:
	GL_Render();
	~GL_Render();

	// do not copy
	GL_Render(const GL_Render&);
	GL_Render& operator=(const GL_Render&);

	void attachShader(const char* program, const char* path, GL_Program::ShaderType type);
	void setActiveProgram(const char* name);

	typedef std::map<std::string, GL_Program> GL_ProgramMap;
	GL_ProgramMap mProgramMap;
	GL_Program* mActiveProgram;
	
	GLenum mDrawMode;
	GLuint mTextureId;
	GLenum mBlendSFactor;
	GLenum mBlendDFactor;
	bool mBlendingEnabled;
	Matrix44f mProjection;

	GLshort mBatchNextIndex;
	RenderBatch* mBatch;

	// statistics
	int mDrawCalls;
};
