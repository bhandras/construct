#include "../common.h"
#include "gl.h"

#include <iostream>
#include <fstream>


GLRenderer* GLRenderer::mInstance = 0;


GLRenderer::GLRenderer()
: mActiveProgram(0)
{ }


GLRenderer::~GLRenderer()
{ }


void GLRenderer::init()
{
	attachShader("xyz_rgba", "xyz_rgba.vs", GLProgram::VertexShader);
	attachShader("xyz_rgba", "xyz_rgba.fs", GLProgram::FragmentShader);

	attachShader("xyz_rgba_uv", "xyz_rgba_uv.vs", GLProgram::VertexShader);
	attachShader("xyz_rgba_uv", "xyz_rgba_uv.fs", GLProgram::FragmentShader);
}


void GLRenderer::setActiveProgram(const char* name)
{
	ProgramMap::iterator it = mPrograms.find(name);

	if (it != mPrograms.end())
	{
		mActiveProgram = &(it->second);

		if (!mActiveProgram->isLinked())
		{
			mActiveProgram->link();
		}

		mActiveProgram->use();
	}
}


void GLRenderer::attachShader(const char* program, const char* path, GLProgram::ShaderType type)
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

		mPrograms[program].setShader(type, std::string(contents, static_cast<unsigned>(size)));

		delete [] contents;
	}
}


void GLRenderer::setOrthoProjection(float left, float right, float bottom, float top, float znear, float zfar)
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


void GLRenderer::draw(DrawMode mode, const Vertex_Vector_XYZ_RGBA& vertices, const Index_Vector& indices)
{
	XYZ_RGBA_Batch& batch = mVertex_Batches_XYZ_RGBA[mode];

	batch.vertices.insert(batch.vertices.end(), vertices.begin(), vertices.end());
	size_t offset = batch.indices.size();
	batch.indices.insert(batch.indices.end(), indices.begin(), indices.end());

	for (size_t i = batch.indices.size() - 1; i > offset; --i)
	{
		batch.indices[i] += offset;
	}
}


void GLRenderer::draw(DrawMode mode, const Vertex_Vector_XYZ_RGBA_UV& vertices, const Index_Vector& indices, GLuint textureId)
{
	std::list<XYZ_RGBA_UV_Batch>& batchList = mVertex_Batches_XYZ_RGBA_UV[mode];

	foreach(std::list<XYZ_RGBA_UV_Batch>, batchList, batchIter)
	{
		XYZ_RGBA_UV_Batch& batch = *batchIter;

		if (batch.textureId == textureId)
		{
			batch.vertices.insert(batch.vertices.end(), vertices.begin(), vertices.end());
			
			size_t offset = batch.indices.size();
			GLushort maxIndex = 0;
			for (size_t i = 0; i < batch.indices.size(); ++i)
			{
				if (batch.indices[i] > maxIndex)
				{
					maxIndex = batch.indices[i];
				}
			}
			
			batch.indices.insert(batch.indices.end(), indices.begin(), indices.end());

			maxIndex += 1;
			for (size_t i = batch.indices.size() - 1; i >= offset; --i)
			{
				batch.indices[i] += maxIndex;
			}

			// we're done
			return;
		}
	}

	batchList.push_back(XYZ_RGBA_UV_Batch());
	XYZ_RGBA_UV_Batch& batch = batchList.back();
	
	batch.textureId = textureId;
	batch.vertices.insert(batch.vertices.end(), vertices.begin(), vertices.end());
	batch.indices.insert(batch.indices.end(), indices.begin(), indices.end());
}


void GLRenderer::flush()
{
	if (mVertex_Batches_XYZ_RGBA.size() > 0)
	{
		setActiveProgram("xyz_rgba");
		GLuint attribPosition = getActiveProgram()->getAttributeLocation("a_position");
		GLuint attribSourceColor = getActiveProgram()->getAttributeLocation("a_source_color");

		GLuint projectionUniformLocation = mActiveProgram->getUniformLocation("u_projection");
		glUniformMatrix4fv(projectionUniformLocation, 1, 0, mProjection.data());

		foreach(XYZ_RGBA_Batch_Map, mVertex_Batches_XYZ_RGBA, it)
		{
			XYZ_RGBA_Batch& batch = it->second;

			glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZ_RGBA), &batch.vertices[0].position);
			glEnableVertexAttribArray(attribPosition);
			glVertexAttribPointer(attribSourceColor, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex_XYZ_RGBA), &batch.vertices[0].color_rgba);
			glEnableVertexAttribArray(attribSourceColor);

			GLenum glDrawMode;
			switch (it->first)
			{
			case TRIANGLE_STRIP:
				glDrawMode = GL_TRIANGLE_STRIP;
				break;
			case TRIANGLE_FAN:
				glDrawMode = GL_TRIANGLE_FAN;
				break;
			case TRIANGLES:
				glDrawMode = GL_TRIANGLES;
				break;
			case LINE_STRIP:
				glDrawMode = GL_LINE_STRIP;
				break;
			case LINES:
				glDrawMode = GL_LINES;
				break;
			}

			glDrawElements(glDrawMode, batch.indices.size(), GL_UNSIGNED_SHORT, &batch.indices[0]); 

			batch.vertices.clear();
			batch.indices.clear();
		}

		mVertex_Batches_XYZ_RGBA.clear();
	}


	if (mVertex_Batches_XYZ_RGBA_UV.size() > 0)
	{
		setActiveProgram("xyz_rgba_uv");
		GLuint attribPosition = mActiveProgram->getAttributeLocation("a_position");
		GLuint attribSourceColor = mActiveProgram->getAttributeLocation("a_source_color");
		GLuint attribTextureCoor = mActiveProgram->getAttributeLocation("a_texture_coor");

		GLuint projectionUniformLocation = mActiveProgram->getUniformLocation("u_projection");
		GLuint textureUniform = mActiveProgram->getUniformLocation("u_texture");

		glUniformMatrix4fv(projectionUniformLocation, 1, 0, mProjection.data());

		foreach(XYZ_RGBA_UV_Batch_Map, mVertex_Batches_XYZ_RGBA_UV, it)
		{
			std::list<XYZ_RGBA_UV_Batch>& batchList = it->second;

			foreach(std::list<XYZ_RGBA_UV_Batch>, batchList, batchIter)
			{
				XYZ_RGBA_UV_Batch& batch = *batchIter;

				glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_XYZ_RGBA_UV), &batch.vertices[0].position);
				glEnableVertexAttribArray(attribPosition);
				glVertexAttribPointer(attribSourceColor, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex_XYZ_RGBA_UV), &batch.vertices[0].color_rgba);
				glEnableVertexAttribArray(attribSourceColor);
				glVertexAttribPointer(attribTextureCoor, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex_XYZ_RGBA_UV), &batch.vertices[0].texture_uv);
				glEnableVertexAttribArray(attribTextureCoor);

				GLenum glDrawMode;
				switch (it->first)
				{
				case TRIANGLE_STRIP:
					glDrawMode = GL_TRIANGLE_STRIP;
					break;
				case TRIANGLE_FAN:
					glDrawMode = GL_TRIANGLE_FAN;
					break;
				case TRIANGLES:
					glDrawMode = GL_TRIANGLES;
					break;
				case LINE_STRIP:
					glDrawMode = GL_LINE_STRIP;
					break;
				case LINES:
					glDrawMode = GL_LINES;
					break;
				}

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, batch.textureId);
				glUniform1i(textureUniform, 0);

				glDrawElements(glDrawMode, batch.indices.size(), GL_UNSIGNED_SHORT, &batch.indices[0]);

				batch.vertices.clear();
				batch.indices.clear();
			}
		}

		mVertex_Batches_XYZ_RGBA_UV.clear();
	}
}
