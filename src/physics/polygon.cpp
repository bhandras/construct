#include "construct.h"


namespace Construct
{
	Polygon::Polygon()
		: Shape(Shape::PolygonShape) 
	{ }


	void Polygon::update()
	{
		size_t numEdges = mEdges.size();

		for (size_t i = 0; i < numEdges; ++i)
		{
			mTransformation.transform(mEdges[i]);
		}

		for (size_t i = 0; i < numEdges; ++i)
		{
			mNormals[i] = mEdges[(i + 1) % numEdges] - mEdges[i];
			mNormals[i].m90();
			mNormals[i].normalize();
		}
	}


	void Polygon::draw()
	{
		Vertex_Vector_XYZ_RGBA vertices;
		vertices.resize(mEdges.size());

		for (size_t i = 0; i < mEdges.size(); ++i)
		{
			vertices[i].setPosition(mEdges[i]);
		}

		Index_Vector indices;
		indices.resize((mEdges.size() - 2) * 3);

		size_t j = 1; 
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			indices[i] = 0;
			indices[i + 1] = j;
			indices[i + 2] = j + 1;
			++j;
		}

		GL_Render& gl = GL_Render::get();

		if (Context::isFilled())
		{
			const Color4 c = Context::getFillColor();
			for (size_t i = 0; i < vertices.size(); ++i)
			{
				vertices[i].setColor(c);
			}

			gl.setDrawMode(GL_TRIANGLES);
			gl.draw_XYZ_RGBA(vertices, indices);
		}

		if (Context::isOutlined())
		{
			const Color4 c = Context::getOutlineColor();
			for (size_t i = 0; i < vertices.size(); ++i)
			{
				vertices[i].setColor(c);
			}

			gl.setDrawMode(GL_LINE_LOOP);
			gl.draw_XYZ_RGBA(vertices, indices);
		}
	}
}
