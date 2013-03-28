#include "SequenceComparator.h"

SequenceComparator::SequenceComparator(void)
{
}

SequenceComparator::SequenceComparator(shared_ptr<NonlinearFunctor> f) :
	_functor(f)
{
}

SequenceComparator::SequenceComparator(SimilarityMatrix mat) :
	_similarity(mat)
{
}

SequenceComparator::SequenceComparator(shared_ptr<NonlinearFunctor> f, SimilarityMatrix mat) :
	_functor(f), _similarity(mat)
{
}

void SequenceComparator::setFunctor(shared_ptr<NonlinearFunctor> f) {
	_functor = f;
}

void SequenceComparator::setMatrix(SimilarityMatrix mat) {
	_similarity = mat;
}

typedef struct {
	int previous;
	int value;
} Cell;

void reverseString(string &s) {
	string result;
	for (int i = 0; i < s.size() / 2; i++) {
		swap(s[i], s[s.size()-i-1]);
	}
}

SequenceComparator::CompareResult SequenceComparator::compare(string &sA, string &sB, string &sC) {

#define COORDS(a, b, c) ((a) + (b)*aLength + (c)*abLength)

	const int aLength = sA.size() + 1;
	const int bLength = sB.size() + 1;
	const int cLength = sC.size() + 1;
	const int abLength = aLength * bLength;
	Cell* F = new Cell[aLength * bLength * cLength];
	int newMax[7];
	int previousLUT[] = { 
		-1-aLength-abLength, -1-aLength, -1-abLength, -1, -aLength-abLength, -aLength, -abLength 
	};

	for (int a = 0; a < aLength; a++) {
		F[COORDS(a, 0, 0)].value = _functor->value(a, 0, 0);
		F[COORDS(a, 0, 0)].previous = COORDS(a-1, 0, 0);
	}
	for (int b = 1; b < bLength; b++) {
		F[COORDS(0, b, 0)].value = _functor->value(0, b, 0);
		F[COORDS(0, b, 0)].previous = COORDS(0, b-1, 0);
	}
	for (int c = 1; c < cLength; c++) {
		F[COORDS(0, 0, c)].value = _functor->value(0, 0, c);
		F[COORDS(0, 0, c)].previous = COORDS(0, 0, c-1);
	}
	for (int a = 1; a < aLength; a++) {
		for (int b = 1; b < bLength; b++) {
			F[COORDS(a, b, 0)].value = _functor->value(a, b, 0);
			F[COORDS(a, b, 0)].previous = COORDS(a-1, b-1, 0);
		}
	}
	for (int a = 1; a < aLength; a++) {
		for (int c = 1; c < cLength; c++) {
			F[COORDS(a, 0, c)].value = _functor->value(a, 0, c);
			F[COORDS(a, 0, c)].previous = COORDS(a-1, 0, c-1);
		}
	}
	for (int c = 1; c < cLength; c++) {
		for (int b = 1; b < bLength; b++) {
			F[COORDS(0, b, c)].value = _functor->value(0, b, c);
			F[COORDS(0, b, c)].previous = COORDS(0, b-1, c-1);
		}
	}

	for (int c = 1; c < cLength; c++) {
		for (int b = 1; b < bLength; b++) {
			for (int a = 1; a < aLength; a++) {
				newMax[0] = F[COORDS(a-1, b-1, c-1)].value + _similarity(sA[a-1], sB[b-1], sC[c-1]);
				newMax[1] = F[COORDS(a-1, b-1, c)].value + _similarity(sA[a-1], sB[b-1], '-');
				newMax[2] = F[COORDS(a-1, b, c-1)].value + _similarity(sA[a-1], '-', sC[c-1]);
				newMax[3] = F[COORDS(a-1, b, c)].value + _similarity(sA[a-1], '-', '-');
				newMax[4] = F[COORDS(a, b-1, c-1)].value + _similarity('-', sB[b-1], sC[c-1]);
				newMax[5] = F[COORDS(a, b-1, c)].value + _similarity('-', sB[b-1], '-');
				newMax[6] = F[COORDS(a, b, c-1)].value + _similarity('-', '-', sC[c-1]);
					
				const int prevIndex = maxElementIndex(newMax, 7);
				F[COORDS(a, b, c)].value = newMax[prevIndex];
				F[COORDS(a, b, c)].previous = COORDS(a, b, c) + previousLUT[prevIndex];
			}
		}
	}

	CompareResult result;
	int index = COORDS(aLength-1, bLength-1, cLength-1);
	while (index != 0) {
		const int diff = F[index].previous - index;
		const int c = index / abLength;
		const int b = (index - c * abLength) / aLength;
		const int a = index - c * abLength - b * aLength;

		if (diff == previousLUT[0]) {
			result.sA += sA[a-1];
			result.sB += sB[b-1];
			result.sC += sC[c-1];
		} else if (diff == previousLUT[1]) {
			result.sA += sA[a-1];
			result.sB += sB[b-1];
			result.sC += '-';
		} else if (diff == previousLUT[2]) {
			result.sA += sA[a-1];
			result.sB += '-';
			result.sC += sC[c-1];
		} else if (diff == previousLUT[3]) {
			result.sA += sA[a-1];
			result.sB += '-';
			result.sC += '-';
		} else if (diff == previousLUT[4]) {
			result.sA += '-';
			result.sB += sB[b-1];
			result.sC += sC[c-1];
		} else if (diff == previousLUT[5]) {
			result.sA += '-';
			result.sB += sB[b-1];
			result.sC += '-';
		} else if (diff == previousLUT[6]) {
			result.sA += '-';
			result.sB += '-';
			result.sC += sC[c-1];
		} else {

		}
		index = F[index].previous;
	}
	
	reverseString(result.sA);
	reverseString(result.sB);
	reverseString(result.sC);
	result.value = F[COORDS(aLength-1, bLength-1, cLength-1)].value;

	return result;
}

int SequenceComparator::maxElementIndex(int *tab, int count) {
	int index = 0;
	for (int i = 1; i < count; i++) {
		if (tab[i] > tab[index]) {
			index = i;
		}
	}
	return index;
}
