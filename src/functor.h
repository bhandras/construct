#pragma once

template <class Function>
class IFunctor;


template <class RT>
class IFunctor<RT()>
{
public:
	virtual ~IFunctor() {};
	virtual RT call()= 0;
	RT operator()() { return call(); }
};


template <class RT, class P1>
class IFunctor<RT(P1)>
{
public:
	virtual ~IFunctor() {};
	virtual RT call(P1) = 0;
	RT operator()(P1 p1) { return call(p1); }
};


template <class RT, class P1, class P2>
class IFunctor<RT(P1, P2)>
{
public:
	virtual ~IFunctor() {};
	virtual RT call(P1, P2) = 0;
	RT operator()(P1 p1, P2 p2) { return call(p1, p2); }
};


template <class RT, class P1, class P2, class P3>
class IFunctor<RT(P1, P2, P3)>
{
public:
	virtual ~IFunctor() {};
	virtual RT call(P1, P2, P3) = 0;
	RT operator()(P1 p1, P2 p2, P3 p3) { return call(p1, p2, p3); }
};


template <class RT, class P1, class P2, class P3, class P4>
class IFunctor<RT(P1, P2, P3, P4)>
{
public:
	virtual ~IFunctor() {};
	virtual RT call(P1, P2, P3, P4) = 0;
	RT operator()(P1 p1, P2 p2, P3 p3, P4 p4) { return call(p1, p2, p3, p4); }
};


template <class RT, class P1, class P2, class P3, class P4, class P5>
class IFunctor<RT(P1, P2, P3, P4, P5)>
{
public:
	virtual ~IFunctor() {};
	virtual RT call(P1, P2, P3, P4, P5) = 0;
	RT operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) { return call(p1, p2, p3, p4, p5); }
};


template <class BASE, class RT>
class Functor;


template <class BASE, class RT>
class Functor<BASE, RT (BASE::*)()> : public IFunctor<RT()>
{
	typedef RT (BASE::*FunctionType)();

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call() { return (m_baseClass->*m_memberFunction)(); }
};


template <class BASE, class RT>
class Functor<BASE, RT (BASE::*)() const> : public IFunctor<RT()>
{
	typedef RT (BASE::*FunctionType)() const;

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call() { return (m_baseClass->*m_memberFunction)(); }
};


template <class BASE, class RT, class P1>
class Functor<BASE, RT (BASE::*)(P1)> : public IFunctor<RT(P1)>
{
	typedef RT (BASE::*FunctionType)(P1);

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call(P1 Param) { return (m_baseClass->*m_memberFunction)(Param); }
};


template <class BASE, class RT, class P1>
class Functor<BASE, RT (BASE::*)(P1) const> : public IFunctor<RT(P1)>
{
	typedef RT (BASE::*FunctionType)(P1) const;

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call(P1 Param) { return (m_baseClass->*m_memberFunction)(Param); }
};


template <class BASE, class RT, class P1, class P2>
class Functor<BASE, RT (BASE::*)(P1, P2)> : public IFunctor<RT(P1, P2)>
{
	typedef RT (BASE::*FunctionType)(P1, P2);

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call(P1 Param1, P2 Param2) { return (m_baseClass->*m_memberFunction)(Param1, Param2); }
};


template <class BASE, class RT, class P1, class P2>
class Functor<BASE, RT (BASE::*)(P1, P2) const> : public IFunctor<RT(P1, P2)>
{
	typedef RT (BASE::*FunctionType)(P1, P2) const;

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call(P1 Param1, P2 Param2) { return (m_baseClass->*m_memberFunction)(Param1, Param2); }
};


template <class BASE, class RT, class P1, class P2, class P3>
class Functor<BASE, RT (BASE::*)(P1, P2, P3)> : public IFunctor<RT(P1, P2, P3)>
{
	typedef RT (BASE::*FunctionType)(P1, P2, P3);

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call(P1 Param1, P2 Param2, P3 Param3) { return (m_baseClass->*m_memberFunction)(Param1, Param2, Param3); }
};


template <class BASE, class RT, class P1, class P2, class P3>
class Functor<BASE, RT (BASE::*)(P1, P2, P3) const> : public IFunctor<RT(P1, P2, P3)>
{
	typedef RT (BASE::*FunctionType)(P1, P2, P3) const;

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call(P1 Param1, P2 Param2, P3 Param3) { return (m_baseClass->*m_memberFunction)(Param1, Param2, Param3); }
};


template <class BASE, class RT, class P1, class P2, class P3, class P4>
class Functor<BASE, RT (BASE::*)(P1, P2, P3, P4)> : public IFunctor<RT(P1, P2, P3, P4)>
{
	typedef RT (BASE::*FunctionType)(P1, P2, P3, P4);

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call(P1 Param1, P2 Param2, P3 Param3, P4 Param4) { return (m_baseClass->*m_memberFunction)(Param1, Param2, Param3, Param4); }
};


template <class BASE, class RT, class P1, class P2, class P3, class P4>
class Functor<BASE, RT (BASE::*)(P1, P2, P3, P4) const> : public IFunctor<RT(P1, P2, P3, P4)>
{
	typedef RT (BASE::*FunctionType)(P1, P2, P3, P4) const;

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call(P1 Param1, P2 Param2, P3 Param3, P4 Param4) { return (m_baseClass->*m_memberFunction)(Param1, Param2, Param3, Param4); }
};


template <class BASE, class RT, class P1, class P2, class P3, class P4, class P5>
class Functor<BASE, RT (BASE::*)(P1, P2, P3, P4, P5)> : public IFunctor<RT(P1, P2, P3, P4, P5)>
{
	typedef RT (BASE::*FunctionType)(P1, P2, P3, P4, P5);

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call(P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5) { return (m_baseClass->*m_memberFunction)(Param1, Param2, Param3, Param4, Param5); }
};


template <class BASE, class RT, class P1, class P2, class P3, class P4, class P5>
class Functor<BASE, RT (BASE::*)(P1, P2, P3, P4, P5) const> : public IFunctor<RT(P1, P2, P3, P4, P5)>
{
	typedef RT (BASE::*FunctionType)(P1, P2, P3, P4, P5) const;

	BASE* m_baseClass;
	FunctionType m_memberFunction;

public:
	Functor(BASE* baseClass, FunctionType memberFunction)
		: m_baseClass(baseClass), m_memberFunction(memberFunction) {}
	virtual ~Functor() {}

	virtual RT call(P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5) { return (m_baseClass->*m_memberFunction)(Param1, Param2, Param3, Param4, Param5); }
};


template <class BASE, class Function>
Functor<BASE, Function>* createFunctor(BASE *baseClass, Function memberFunction) 
{
	return new Functor<BASE, Function>(baseClass, memberFunction);
}


template <class BASE, class Function>
Functor<BASE, Function>* createFunctor(const BASE *baseClass, Function memberFunction) 
{
	return new Functor<BASE, Function>(baseClass, memberFunction);
}

