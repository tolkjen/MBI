#include "SimilarityMatrix.h"

int SimilarityMatrix::operator() (Alphabet a, Alphabet b,
						Alphabet c) const {
	if (a == b && b == c) {
		return 1;
	} else {
		return -1;
	}
}
