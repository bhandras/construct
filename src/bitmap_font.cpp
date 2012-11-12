#include "construct.h"


// Maximum string length
#define BFG_MAXSTRING 255
// Offset to width data with BFF file
#define WIDTH_DATA_OFFSET 20
// Offset to texture image data with BFF file
#define MAP_DATA_OFFSET 276

// This definition is missing from some GL libs
#ifndef GL_CLAMP_TO_EDGE 
#define GL_CLAMP_TO_EDGE 0x812F
#endif


BitmapFont::BitmapFont()
: mColor(1.0f, 1.0f, 1.0f, 1.0f)
{ }


BitmapFont::~BitmapFont()
{ }


bool BitmapFont::load(const std::string& path)
{
	std::fstream fontFile;
	fontFile.open(path.c_str(), std::ios_base::binary | std::ios_base::in);

	if(fontFile.fail())
	{
		return false;
	}

	// get Filesize
	fontFile.seekg(0, std::ios_base::end);
	std::streamoff fileSize = fontFile.tellg();
	fontFile.seekg(0, std::ios_base::beg);

	char* data = new char[static_cast<unsigned int>(fileSize)];

	if (!data)
	{
		assert(false);
		return false;
	}

	// read filedata
	fontFile.read(data, fileSize);

	if (fontFile.fail())
	{
		delete [] data;
		fontFile.close();
		return false;
	}

	fontFile.close();

	// check ID is 'BFF2'
	if(static_cast<unsigned char>(data[0]) != 0xBF || static_cast<unsigned char>(data[1]) != 0xF2)
	{
		assert(false);
		delete [] data;
		return false;
	}

	// grab the rest of the header
	int imgX, imgY;
	memcpy(&imgX, &data[2], sizeof(int));
	memcpy(&imgY, &data[6], sizeof(int));
	memcpy(&mCellSizeX, &data[10], sizeof(int));
	memcpy(&mCellSizeY, &data[14], sizeof(int));
	int bitsPerPixel;

	bitsPerPixel = static_cast<int>(data[18]);
	mBase = data[19];

	// check filesize
	if (fileSize != ((MAP_DATA_OFFSET) + ((imgX * imgY) * (bitsPerPixel / 8))))
	{
		assert(false);
		return false;
	}

	// calculate font params
	mRowPitch = imgX / mCellSizeX;
	mColFactor = static_cast<float>(mCellSizeX) / static_cast<float>(imgX);
	mRowFactor = static_cast<float>(mCellSizeY) / static_cast<float>(imgY);
	mOffsetY = mCellSizeY;

	// determine blending options based on BPP
	switch (bitsPerPixel)
	{
	case 8: // Greyscale
		mRenderStyle = BFG_RS_ALPHA;
		break;

	case 24: // RGB
		mRenderStyle = BFG_RS_RGB;
		break;

	case 32: // RGBA
		mRenderStyle = BFG_RS_RGBA;
		break;

	default: // Unsupported BPP
		delete [] data;
		assert(false);
		return false;
		break;
	}

	// grab char widths
	memcpy(mCarWidthTable, &data[WIDTH_DATA_OFFSET], 256);

	// create Texture
	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	
	// Fonts should be rendered at native resolution so no need for texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	// Stop chararcters from bleeding over edges
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// texture creation params are dependent on BPP
	switch (mRenderStyle)
	{
	case BFG_RS_ALPHA:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, imgX, imgY, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE, &data[MAP_DATA_OFFSET]);
		break;

	case BFG_RS_RGB:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgX, imgY, 0, GL_RGB,GL_UNSIGNED_BYTE, &data[MAP_DATA_OFFSET]);
		break;

	case BFG_RS_RGBA:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgX, imgY, 0, GL_RGBA,GL_UNSIGNED_BYTE, &data[MAP_DATA_OFFSET]);
		break;
	}

	// clean up
	delete [] data;
	return true;
}


float BitmapFont::getTextWidth(const std::string& text) const
{
	float textWidth = 0.0f;
	for (size_t i = 0; i < text.length(); ++i)
	{
		textWidth += static_cast<float>(mCarWidthTable[text[i]]);
	}

	return textWidth;
}


float BitmapFont::getFontSize() const
{
	return static_cast<float>(mCellSizeY);
}


void BitmapFont::drawString(const std::string& text, float x, float y, Alignment alignment) const
{	
	float textWidth = getTextWidth(text);
	
	switch (alignment)
	{
	case ALIGN_TL:
		y += mCellSizeY;
		break;
	case ALIGN_TR:
		x -= textWidth;
		break;
	case ALIGN_BL:
		break;
	case ALIGN_BR:
		x -= textWidth;
		break;
	case ALIGN_CENTER:
		x -= textWidth / 2;
		y -= mCellSizeY / 2;
		break;
	default:
		assert(false);
		break;
	}

	size_t textLength = text.length();
	
	Index_Vector indices;
	Vertex_Vector_XYZ_RGBA_UV vertices;
	vertices.resize(textLength * 4);
	indices.resize(textLength * 6);

	for (size_t i = 0; i < textLength; ++i)
	{
		int row = (text[i] - mBase) / mRowPitch;
		int col = (text[i] - mBase) - row * mRowPitch;

		float u1 = col * mColFactor;
		float v1 = row * mRowFactor;
		float u = u1 + mColFactor;
		float v = v1 + mRowFactor;

		size_t offset = i * 4;

		vertices[offset + 0].position[0] = x;
		vertices[offset + 0].position[1] = y;
		vertices[offset + 0].texture_uv[0] = u1;
		vertices[offset + 0].texture_uv[1] = v1;
		vertices[offset + 0].setColor(mColor);

		vertices[offset + 1].position[0] = x + mCellSizeX;
		vertices[offset + 1].position[1] = y;
		vertices[offset + 1].texture_uv[0] = u;
		vertices[offset + 1].texture_uv[1] = v1;
		vertices[offset + 1].setColor(mColor);

		vertices[offset + 2].position[0] = x;
		vertices[offset + 2].position[1] = y + mCellSizeY;
		vertices[offset + 2].texture_uv[0] = u1;
		vertices[offset + 2].texture_uv[1] = v;
		vertices[offset + 2].setColor(mColor);

		vertices[offset + 3].position[0] = x + mCellSizeX;
		vertices[offset + 3].position[1] = y + mCellSizeY;
		vertices[offset + 3].texture_uv[0] = u;
		vertices[offset + 3].texture_uv[1] = v;
		vertices[offset + 3].setColor(mColor);

		size_t index = i * 6;
		indices[index + 0] = offset + 0;
		indices[index + 1] = offset + 1;
		indices[index + 2] = offset + 2;
		indices[index + 3] = offset + 1;
		indices[index + 4] = offset + 2;
		indices[index + 5] = offset + 3;

		x += mCarWidthTable[text[i]];
	}

	GL_Render& gl = GL_Render::get();

	switch (mRenderStyle)
	{
		// 8 bit
	case BFG_RS_ALPHA:
		gl.enableBlending(GL_SRC_ALPHA, GL_SRC_ALPHA);
		break;

		// 24 bit
	case BFG_RS_RGB:
		gl.disableBlending();
		break;

		// 32 bit
	case BFG_RS_RGBA:
		gl.enableBlending(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		break;
	}

	gl.setTexture(mTextureId);
	gl.setDrawMode(GL_TRIANGLES);
	gl.draw_XYZ_RGBA_UV(vertices, indices);
}
