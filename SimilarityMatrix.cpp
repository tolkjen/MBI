/*
 * MBI projekt, semestr 13L
 *
 * Tomasz Figa, Adam Stelmaszczyk, Piotr Takiel
 *
 * Badanie podobienstwa trzech sekwencji z nieliniowa kara za przesuniecie
 * sekwencji wzgledem siebie.
 */

#include "SimilarityMatrix.h"

int SimpleSimilarityMatrix::operator() (Alphabet a, Alphabet b,
						Alphabet c) const {
	if (a == b && b == c) {
		return 1;
	} else {
		return -1;
	}
}

CustomSimilarityMatrix::CustomSimilarityMatrix(
				const int data[MAX_SYMBOL][MAX_SYMBOL],
				int pausePenalty) {
	for (int x = 0; x < MAX_SYMBOL; ++x)
		for (int y = 0; y < MAX_SYMBOL; ++y)
			_data[x][y] = data[x][y];

	for (int x = 0; x < MAX_SYMBOL; ++x)
		_data[x][MAX_SYMBOL] = pausePenalty;

	for (int y = 0; y < MAX_SYMBOL; ++y)
		_data[MAX_SYMBOL][y] = pausePenalty;

	_data[MAX_SYMBOL][MAX_SYMBOL] = 0;
}

int CustomSimilarityMatrix::operator() (Alphabet a, Alphabet b,
							Alphabet c) const {
	return _data[a][b] + _data[a][c] + _data[b][c];
}
