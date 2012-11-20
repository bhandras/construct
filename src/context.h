#pragma once


class Context
{
public:
	static void setFilled(bool isFilled);
	static void setFillColor(const Color4& fillColor);

	static bool isFilled();
	static const Color4& getFillColor();

	static void setOutlined(bool isOutlined);
	static void setOutlineColor(const Color4& outlineColor);

	static bool isOutlined();
	static const Color4& getOutlineColor();

	static void push();
	static void pop();

private:
	struct ContextData
	{
		ContextData() : isFilled(false), isOutlined(false) { }

		bool isFilled;
		Color4 fillColor;
		bool isOutlined;
		Color4 outlineColor;
	};

	static std::stack<ContextData> mStack;
};
