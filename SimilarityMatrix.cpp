#include "SimilarityMatrix.h"

int SimilarityMatrix::operator() (char a, char b, char c) {
	if (a == b && b == c) {
		return 1;
	} else {
		return -1;
	}
}