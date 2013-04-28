#ifndef NONLINEAR_FUNCTOR_H
#define NONLINEAR_FUNCTOR_H

class NonlinearFunctor
{
public:
	virtual int value(const int a, const int b, const int c) = 0;
	virtual ~NonlinearFunctor(){};
};

#endif
