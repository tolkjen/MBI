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
	int operator() (Alphabet a, Alphabet b, Alphabet c) const;
};

#endif
