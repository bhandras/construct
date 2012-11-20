#include "construct.h"


std::stack<Context::ContextData> Context::mStack;


void Context::setFilled(bool isFilled)
{
	mStack.top().isFilled = isFilled;
}


void Context::setFillColor(const Color4& fillColor)
{
	mStack.top().fillColor = fillColor;
}


bool Context::isFilled()
{
	return mStack.top().isFilled;
}


const Color4& Context::getFillColor()
{
	return mStack.top().fillColor;
}


void Context::setOutlined(bool isOutlined)
{
	mStack.top().isOutlined = isOutlined;
}


void Context::setOutlineColor(const Color4& outlineColor)
{
	mStack.top().outlineColor = outlineColor;
}


bool Context::isOutlined()
{
	return mStack.top().isOutlined;
}


const Color4& Context::getOutlineColor()
{
	return mStack.top().outlineColor;
}


void Context::push()
{
	mStack.push(ContextData());
}


void Context::pop()
{
	mStack.pop();
}
