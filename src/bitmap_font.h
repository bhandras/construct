#pragma once


class BitmapFont
{
public:
	enum Alignment
	{
		ALIGN_TL,
		ALIGN_TR,
		ALIGN_BL,
		ALIGN_BR,
		ALIGN_CENTER
	};

	BitmapFont();
	~BitmapFont();

	bool load(const std::string& path);

	float getTextWidth(const std::string& text) const;
	float getFontSize() const;

	void setColor(const Color4& color) { mColor = color; }
	void drawString(const std::string& text, float x, float y, Alignment alignment) const;

private:
	// blend flags
	enum
	{
		BFG_RS_NONE = 0x0,
		BFG_RS_ALPHA = 0x1,
		BFG_RS_RGB = 0x2,
		BFG_RS_RGBA = 0x4
	};

	int mCellSizeX, mCellSizeY, mOffsetY, mRowPitch;
	char mBase;
	char mCarWidthTable[256];
	float mRowFactor, mColFactor;
	int mRenderStyle;
	GLuint mTextureId;
	Color4 mColor;
};
