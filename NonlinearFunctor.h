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

/**
 * A non-linear functor class.
 * Represents a non-linear function that is used to calculate score of
 * shifting sequences relative to themselves.
 */
class NonlinearFunctor
{
public:
	/**
	 * Returns value of non-linear function for shifts of three sequences.
	 * @param a Shift level of first sequence.
	 * @param b Shift level of second sequence.
	 * @param c Shift level of third sequence.
	 * @return Score of shift operation.
	 */
	virtual int value(const int a, const int b, const int c) = 0;

	/**
	 * Destroys non-linear functor object.
	 */
	virtual ~NonlinearFunctor(){};
};

#endif
