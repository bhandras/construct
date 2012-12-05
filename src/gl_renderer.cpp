#include "construct.h"


GL_Render::GL_Render()
: mActiveProgram(0)
, mDrawMode(0)
, mTextureId(0)
, mBatchNextIndex(0)
, mDrawCalls(0)
{ }


GL_Render::~GL_Render()
{ }


void GL_Render::init()
{
	mProgramMap["xyz_rgba"].setShader(GL_Program::VertexShader, xyz_rgba_vs);
	mProgramMap["xyz_rgba"].setShader(GL_Program::FragmentShader, xyz_rgba_fs);
	mProgramMap["xyz_rgba_uv"].setShader(GL_Program::VertexShader, xyz_rgba_uv_vs);
	mProgramMap["xyz_rgba_uv"].setShader(GL_Program::FragmentShader, xyz_rgba_uv_fs);
}


void GL_Render::beginFrame()
{
	mDrawCalls = 0;
}


void GL_Render::endFrame()
{
	flush();
}


void GL_Render::setDrawMode(GLenum drawMode)
{
	if (mDrawMode != drawMode)
	{
		flush();
	}

	mDrawMode = drawMode;
}


void GL_Render::setTexture(GLuint textureId)
{
	if (mTextureId != textureId)
	{
		flush();
	}

	mTextureId = textureId;
}


void GL_Render::enableBlending(GLenum sFactor, GLenum dFactor)
{
	if (!mBlendingEnabled || mBlendSFactor != sFactor || mBlendDFactor != dFactor)
	{
		flush();
	}

	mBlendingEnabled = true;
	mBlendSFactor = sFactor;
	mBlendDFactor = dFactor;
	glBlendFunc(sFactor, dFactor);
	glEnable(GL_BLEND);
}


void GL_Render::disableBlending()
{
	if (mBlendingEnabled)
	{
		flush();
	}

	mBlendingEnabled = false;
	glDisable(GL_BLEND);
}


void GL_Render::setOrthoProjection(float left, float right, float bottom, float top, float znear, float zfar)
{
	mProjection.identity();
	mProjection(0, 0) = 2.0f / (right - left);
	mProjection(1, 1) = 2.0f / (top - bottom);
	mProjection(2, 2) = -2.0f / (zfar - znear);
	

	mProjection(3, 0) = -(right + left) / (right - left);
	mProjection(3, 1) = -(top + bottom) / (top - bottom);
	mProjection(3, 2) = -(zfar + znear) / (zfar - znear);
	mProjection(3, 3) = 1.0f;
}


void GL_Render::draw_XYZ_RGBA(const Vertex_Vector_XYZ_RGBA& vertices, const Index_Vector& indices)
{
	if (mBatch && mBatch->type != RenderBatch::T_XYZ_RGBA)
	{
		flush();
	}

	if (!mBatch)
	{
		mBatch = new XYZ_RGBA_RenderBatch;
	}
	
	XYZ_RGBA_RenderBatch* batch = static_cast<XYZ_RGBA_RenderBatch*>(mBatch);
	batch->vertices.insert(batch->vertices.end(), vertices.begin(), vertices.end());

	size_t next = batch->indices.size();
	GLushort maxIndex = mBatchNextIndex;
	batch->indices.resize(batch->indices.size() + indices.size());

	for (size_t i = 0; i < indices.size(); ++i)
	{
		GLushort index = indices[i] + mBatchNextIndex;
		batch->indices[next] = index;
		if (index > maxIndex)
		{
			maxIndex = index;
		}
		++next;
	}
	
	mBatchNextIndex = maxIndex + 1;

	if (mDrawMode != GL_TRIANGLES && mDrawMode != GL_LINES && mDrawMode != GL_POINTS)
	{
		flush();
	}
}


void GL_Render::draw_XYZ_RGBA_UV(const Vertex_Vector_XYZ_RGBA_UV& vertices, const Index_Vector& indices)
{
	if (mBatch && mBatch->type != RenderBatch::T_XYZ_RGBA_UV)
	{
		flush();
	}

	if (!mBatch)
	{
		mBatch = new XYZ_RGBA_UV_RenderBatch;
	}

	XYZ_RGBA_UV_RenderBatch* batch = static_cast<XYZ_RGBA_UV_RenderBatch*>(mBatch);
	batch->vertices.insert(batch->vertices.end(), vertices.begin(), vertices.end());

	size_t next = batch->indices.size();
	GLushort maxIndex = mBatchNextIndex;
	batch->indices.resize(batch->indices.size() + indices.size());

	for (size_t i = 0; i < indices.size(); ++i)
	{
		GLushort index = indices[i] + mBatchNextIndex;
		batch->indices[next] = index;
		if (index > maxIndex)
		{
			maxIndex = index;
		}
		++next;
	}

	mBatchNextIndex = maxIndex + 1;

	if (mDrawMode != GL_TRIANGLES && mDrawMode != GL_LINES && mDrawMode != GL_POINTS)
	{
		flush();
	}
}


void GL_Render::flush()
{
	if (!mBatch)
	{
		// nothing to flush
		return;
	}

	switch (mBatch->type)
	{
	case RenderBatch::T_XYZ_RGBA:
		{
			XYZ_RGBA_RenderBatch* batch = static_cast<XYZ_RGBA_RenderBatch*>(mBatch);

			setActiveProgram("xyz_rgba");
			GLuint attribPosition = mActiveProgram->getAttributeLocation("a_position");
			GLuint attribSourceColor = mActiveProgram->getAttributeLocation("a_source_color");

			GLuint projectionUniformLocation = mActiveProgram->getUniformLocation("u_projection");
			glUniformMatrix4fv(projectionUniformLocation, 1, 0, mProjection.data());

			glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZ_RGBA), &batch->vertices[0].position);
			glEnableVertexAttribArray(attribPosition);
			glVertexAttribPointer(attribSourceColor, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex_XYZ_RGBA), &batch->vertices[0].color_rgba);
			glEnableVertexAttribArray(attribSourceColor);
			
			glDrawElements(mDrawMode, batch->indices.size(), GL_UNSIGNED_SHORT, &batch->indices[0]);

			glDisableVertexAttribArray(attribPosition);
			glDisableVertexAttribArray(attribSourceColor);

			++mDrawCalls;
			break;
		}

	case RenderBatch::T_XYZ_RGBA_UV:
		{
			XYZ_RGBA_UV_RenderBatch* batch = static_cast<XYZ_RGBA_UV_RenderBatch*>(mBatch);

			setActiveProgram("xyz_rgba_uv");
			GLuint attribPosition = mActiveProgram->getAttributeLocation("a_position");
			GLuint attribSourceColor = mActiveProgram->getAttributeLocation("a_source_color");
			GLuint attribTextureCoor = mActiveProgram->getAttributeLocation("a_texture_coor");

			GLuint projectionUniformLocation = mActiveProgram->getUniformLocation("u_projection");
			GLuint textureUniform = mActiveProgram->getUniformLocation("u_texture");

			glUniformMatrix4fv(projectionUniformLocation, 1, 0, mProjection.data());

			glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZ_RGBA_UV), &batch->vertices[0].position);
			glEnableVertexAttribArray(attribPosition);
			glVertexAttribPointer(attribSourceColor, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex_XYZ_RGBA_UV), &batch->vertices[0].color_rgba);
			glEnableVertexAttribArray(attribSourceColor);
			glVertexAttribPointer(attribTextureCoor, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex_XYZ_RGBA_UV), &batch->vertices[0].texture_uv);
			glEnableVertexAttribArray(attribTextureCoor);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mTextureId);
			glUniform1i(textureUniform, 0);

			glDrawElements(mDrawMode, batch->indices.size(), GL_UNSIGNED_SHORT, &batch->indices[0]);

			glDisableVertexAttribArray(attribPosition);
			glDisableVertexAttribArray(attribSourceColor);
			glDisableVertexAttribArray(attribTextureCoor);

			++mDrawCalls;
			break;
		}

	default:
		break;
	}

	mBatchNextIndex = 0;
	delete mBatch;
	mBatch = 0;
}


int GL_Render::getNumDrawCalls() const
{
	return mDrawCalls;
}


void GL_Render::attachShader(const char* program, const char* path, GL_Program::ShaderType type)
{
	std::ifstream f(path, std::ios::binary);
	if (!f.fail())
	{
		f.seekg(0, std::ios::end);
		std::ifstream::pos_type size = f.tellg();
		f.seekg(0, std::ios::beg);

		char* contents = new char[static_cast<size_t>(size)];

		f.read(contents, size);
		f.close();

		mProgramMap[program].setShader(type, std::string(contents, static_cast<unsigned>(size)));

		delete [] contents;
	}
}


void GL_Render::setActiveProgram(const char* name)
{
	GL_ProgramMap::iterator it = mProgramMap.find(name);

	if (it != mProgramMap.end())
	{
		mActiveProgram = &(it->second);

		if (!mActiveProgram->isLinked())
		{
			mActiveProgram->link();
		}

		mActiveProgram->use();
	}
}
