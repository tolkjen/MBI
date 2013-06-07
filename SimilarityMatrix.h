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

/**
 * Defines internal representation of characters supported in sequences.
 */
enum Alphabet {
	SYMBOL_A,
	SYMBOL_C,
	SYMBOL_G,
	SYMBOL_UT,
	SYMBOL_PAUSE,
};

/** Index of last symbol in the alphabet. */
static const int MAX_SYMBOL = SYMBOL_PAUSE;
/** Number of symbols in the alphabet. */
static const int NUM_SYMBOLS = MAX_SYMBOL + 1;

/**
 * Abstract similarity matrix.
 * Represents a way to compare three sequence characters (including pauses).
 */
class SimilarityMatrix
{
public:
	/**
	 * Compares three sequence characters and returns comparison score.
	 * @param a Character from first sequence.
	 * @param b Character from second sequence.
	 * @param c Character from third sequence.
	 * @return Comparison score.
	 */
	virtual int operator() (Alphabet a, Alphabet b, Alphabet c) const = 0;
};

/**
 * Simple similarity matrix.
 * Returns 1 if all three characters matches and -1 if not.
 */
class SimpleSimilarityMatrix : public SimilarityMatrix
{
public:
	virtual int operator() (Alphabet a, Alphabet b, Alphabet c) const;
};

/**
 * Custom similarity matrix.
 * Returns comparison score based pair-wise comparison of three characters
 * using 2-dimensional similarity matrix and pause penalty.
 */
class CustomSimilarityMatrix : public SimilarityMatrix
{
	int _data[NUM_SYMBOLS][NUM_SYMBOLS];
public:
	/**
	 * Constructs a custom similarity matrix.
	 * @param data Matrix of penalty/prize for all alphabet symbols
	 * except pauses.
	 * @param pausePenalty Value of pause penalty.
	 */
	CustomSimilarityMatrix(const int data[MAX_SYMBOL][MAX_SYMBOL],
							int pausePenalty);

	virtual int operator() (Alphabet a, Alphabet b, Alphabet c) const;
};

#endif
