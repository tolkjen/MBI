#include "SequenceComparator.h"
#include <algorithm>

// This macro allows easier access to 3D elements in 1D array
#define COORDS(a, b, c) ((a) + (b)*aLength + (c)*abLength)

SequenceComparator::SequenceComparator(void)
{
}

SequenceComparator::SequenceComparator(shared_ptr<NonlinearFunctor> f) :
	_functor(f)
{
}

SequenceComparator::SequenceComparator(const SimilarityMatrix &mat) :
	_similarity(mat)
{
}

SequenceComparator::SequenceComparator(shared_ptr<NonlinearFunctor> f,
						const SimilarityMatrix &mat) :
	_functor(f), _similarity(mat)
{
}

void SequenceComparator::setFunctor(shared_ptr<NonlinearFunctor> f) {
	_functor = f;
}

void SequenceComparator::setMatrix(const SimilarityMatrix &mat) {
	_similarity = mat;
}

SequenceComparator::CompareResult SequenceComparator::compare(
			const vector<Alphabet> &sA, const vector<Alphabet> &sB,
			const vector<Alphabet> &sC) {
	const int aLength = sA.size() + 1;
	const int bLength = sB.size() + 1;
	const int cLength = sC.size() + 1;
	const int abLength = aLength * bLength;

	// newMax is used to hold 7 values from which we pick the
	// greatest using maxElementIndex().
	// I do it instead of max(max(max( .... ), v5), v6).
	int newMax[7];
	// previousLUT holds information about distance (in terms
	// of linear memory of array F) between
	// a cube and 7 other cubes which are behind it.
	int previousLUT[] = { 
		-1-aLength-abLength, -1-aLength, -1-abLength, -1,
		-aLength-abLength, -aLength, -abLength
	};
	// This array holds info about value and predecessor of each cell (cube).
	Cell* F = new Cell[aLength * bLength * cLength];
	
	// Fill three walls of the cube with values supplied by
	// NonlinearFunctor object.
	createBorders(F, aLength, bLength, cLength);

	// Fill entire structure
	for (int c = 1; c < cLength; c++) {
		for (int b = 1; b < bLength; b++) {
			for (int a = 1; a < aLength; a++) {
				newMax[0] = F[COORDS(a-1, b-1, c-1)].value
					+ _similarity(sA[a-1], sB[b-1], sC[c-1]);
				newMax[1] = F[COORDS(a-1, b-1, c)].value
					+ _similarity(sA[a-1], sB[b-1], SYMBOL_PAUSE);
				newMax[2] = F[COORDS(a-1, b, c-1)].value
					+ _similarity(sA[a-1], SYMBOL_PAUSE, sC[c-1]);
				newMax[3] = F[COORDS(a-1, b, c)].value
					+ _similarity(sA[a-1], SYMBOL_PAUSE, SYMBOL_PAUSE);
				newMax[4] = F[COORDS(a, b-1, c-1)].value
					+ _similarity(SYMBOL_PAUSE, sB[b-1], sC[c-1]);
				newMax[5] = F[COORDS(a, b-1, c)].value
					+ _similarity(SYMBOL_PAUSE, sB[b-1], SYMBOL_PAUSE);
				newMax[6] = F[COORDS(a, b, c-1)].value
					+ _similarity(SYMBOL_PAUSE, SYMBOL_PAUSE, sC[c-1]);
					
				const int prevIndex = maxElementIndex(newMax, 7);
				F[COORDS(a, b, c)].value = newMax[prevIndex];
				F[COORDS(a, b, c)].previous = COORDS(a, b, c)
						+ previousLUT[prevIndex];
			}
		}
	}

	// When the array is filled with data, extract information
	// about sequence match and match value.
	CompareResult result = getResultFromArray(F, sA, sB, sC);

	// Free memory
	delete [] F;

	return result;
}

void SequenceComparator::createBorders(Cell *F,
				int aLength, int bLength, int cLength) {
	const int abLength = aLength * bLength;

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
			F[COORDS(a, b, 0)].previous = maxElementIndex(F,
					COORDS(a-1, b, 0), COORDS(a, b-1, 0),
					COORDS(a-1, b-1, 0));
		}
	}
	for (int a = 1; a < aLength; a++) {
		for (int c = 1; c < cLength; c++) {
			F[COORDS(a, 0, c)].value = _functor->value(a, 0, c);
			F[COORDS(a, 0, c)].previous = maxElementIndex(F,
					COORDS(a-1, 0, c), COORDS(a, 0, c-1),
					COORDS(a-1, 0, c-1));
		}
	}
	for (int c = 1; c < cLength; c++) {
		for (int b = 1; b < bLength; b++) {
			F[COORDS(0, b, c)].value = _functor->value(0, b, c);
			F[COORDS(0, b, c)].previous = maxElementIndex(F,
					COORDS(0, b, c-1), COORDS(0, b-1, c),
					COORDS(c, b-1, c-1));
		}
	}
}

SequenceComparator::CompareResult SequenceComparator::getResultFromArray(
				Cell *F, const vector<Alphabet> &sA,
				const vector<Alphabet> &sB,
				const vector<Alphabet> &sC) {
	const int aLength = sA.size() + 1;
	const int bLength = sB.size() + 1;
	const int cLength = sC.size() + 1;
	const int abLength = aLength * bLength;

	int previousLUT[] = { 
		-1-aLength-abLength, -1-aLength, -1-abLength, -1,
		-aLength-abLength, -aLength, -abLength
	};
	CompareResult result;

	// Create three strings going from the last element in the array
	int index = COORDS(aLength-1, bLength-1, cLength-1);
	while (index != 0) {
		const int diff = F[index].previous - index;
		const int c = index / abLength;
		const int b = (index - c * abLength) / aLength;
		const int a = index - c * abLength - b * aLength;

		if (diff == previousLUT[0]) {
			result.sA.push_back(sA[a-1]);
			result.sB.push_back(sB[b-1]);
			result.sC.push_back(sC[c-1]);
		} else if (diff == previousLUT[1]) {
			result.sA.push_back(sA[a-1]);
			result.sB.push_back(sB[b-1]);
			result.sC.push_back(SYMBOL_PAUSE);
		} else if (diff == previousLUT[2]) {
			result.sA.push_back(sA[a-1]);
			result.sB.push_back(SYMBOL_PAUSE);
			result.sC.push_back(sC[c-1]);
		} else if (diff == previousLUT[3]) {
			result.sA.push_back(sA[a-1]);
			result.sB.push_back(SYMBOL_PAUSE);
			result.sC.push_back(SYMBOL_PAUSE);
		} else if (diff == previousLUT[4]) {
			result.sA.push_back(SYMBOL_PAUSE);
			result.sB.push_back(sB[b-1]);
			result.sC.push_back(sC[c-1]);
		} else if (diff == previousLUT[5]) {
			result.sA.push_back(SYMBOL_PAUSE);
			result.sB.push_back(sB[b-1]);
			result.sC.push_back(SYMBOL_PAUSE);
		} else if (diff == previousLUT[6]) {
			result.sA.push_back(SYMBOL_PAUSE);
			result.sB.push_back(SYMBOL_PAUSE);
			result.sC.push_back(sC[c-1]);
		} else {

		}
		index = F[index].previous;
	}
	
	// Strings were created in reverse order, so now we turn them around.
	reverse(result.sA.begin(), result.sA.end());
	reverse(result.sB.begin(), result.sB.end());
	reverse(result.sC.begin(), result.sC.end());

	// Store comparison value
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

int SequenceComparator::maxElementIndex(Cell *F, int i0, int i1, int i2) {
	if (F[i0].value >= F[i1].value && F[i0].value >= F[i2].value) {
		return i0;
	} else if (F[i1].value >= F[i0].value && F[i1].value >= F[i2].value) {
		return i1;
	} else {
		return i2;
	}
}
