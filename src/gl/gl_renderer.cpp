#include "../common.h"
#include "gl.h"

#include <iostream>
#include <fstream>


GL_Render::GL_Render()
: mActiveProgram(0)
, mDrawMode(0)
, mTextureId(0)
{ }


GL_Render::~GL_Render()
{ }


void GL_Render::init()
{
	attachShader("xyz_rgba", "xyz_rgba.vs", GL_Program::VertexShader);
	attachShader("xyz_rgba", "xyz_rgba.fs", GL_Program::FragmentShader);

	attachShader("xyz_rgba_uv", "xyz_rgba_uv.vs", GL_Program::VertexShader);
	attachShader("xyz_rgba_uv", "xyz_rgba_uv.fs", GL_Program::FragmentShader);
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
	batch->indices.insert(batch->indices.end(), indices.begin(), indices.end());
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
	batch->indices.insert(batch->indices.end(), indices.begin(), indices.end());
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
		}
		
	}

	delete mBatch;
	mBatch = 0;
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
	ProgramMap::iterator it = mProgramMap.find(name);

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
