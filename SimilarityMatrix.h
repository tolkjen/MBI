/*
 * MBI projekt, semestr 13L
 *
 * Tomasz Figa, Adam Stelmaszczyk, Piotr Takiel
 *
 * Badanie podobienstwa trzech sekwencji z nieliniowa kara za przesuniecie
 * sekwencji wzgledem siebie.
 */

#ifndef SIMILARITY_MATRIX_H
#define SIMILARITY_MATRIX_H

enum Alphabet {
	SYMBOL_A,
	SYMBOL_C,
	SYMBOL_G,
	SYMBOL_UT,
	SYMBOL_PAUSE,
};

static const int MAX_SYMBOL = SYMBOL_PAUSE;
static const int NUM_SYMBOLS = MAX_SYMBOL + 1;

class SimilarityMatrix
{
public:
	virtual int operator() (Alphabet a, Alphabet b, Alphabet c) const;
};

class CustomSimilarityMatrix : public SimilarityMatrix
{
	int _data[NUM_SYMBOLS][NUM_SYMBOLS];
public:
	CustomSimilarityMatrix(const int data[MAX_SYMBOL][MAX_SYMBOL],
							int pausePenalty);

	virtual int operator() (Alphabet a, Alphabet b, Alphabet c) const;
};

#endif
