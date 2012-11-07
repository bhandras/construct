#pragma once


#include <stack>


template <class T>
class Matrix44Stack
{
public:
	typedef Matrix44<T> Matrix44t;

	Matrix44Stack()
	{
		Matrix44t m;
		m.identity();
		mStack.push(m);
	}

	void push()
	{
		Matrix44t m;
		m.identity();
		mStack.push(m);
	}

	void pop()
	{
		mStack.pop();
	}

	void mul(const Matrix44t& m)
	{
		mStack.top() = mStack.top() * m;
	}

	Matrix44t& get()
	{
		return mStack.top();
	}

	void set(const Matrix44t& m)
	{
		mStack.top() = m;
	}

	void clear()
	{
		while (mStack.size() > 1)
		{
			mStack.pop();
		}
	}

private:
	std::stack<Matrix44t> mStack;
};
