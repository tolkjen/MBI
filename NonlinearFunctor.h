/*
 * MBI projekt, semestr 13L
 *
 * Tomasz Figa, Adam Stelmaszczyk, Piotr Takiel
 *
 * Badanie podobienstwa trzech sekwencji z nieliniowa kara za przesuniecie
 * sekwencji wzgledem siebie.
 */

#ifndef NONLINEAR_FUNCTOR_H
#define NONLINEAR_FUNCTOR_H

class NonlinearFunctor
{
public:
	virtual int value(const int a, const int b, const int c) = 0;
	virtual ~NonlinearFunctor(){};
};

#endif
