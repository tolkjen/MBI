#ifndef SEQUENCE_COMPARATOR_H
#define SEQUENCE_COMPARATOR_H

#include <string>
#include <vector>
#include <memory>
#include <boost/shared_ptr.hpp>
#include "NonlinearFunctor.h"
#include "SimilarityMatrix.h"

using namespace std;
using namespace boost;

class SequenceComparator
{
public:
	typedef struct {
		int value;
		string sA;
		string sB;
		string sC;
	} CompareResult;

	SequenceComparator(void);
	SequenceComparator(shared_ptr<NonlinearFunctor> f);
	SequenceComparator(SimilarityMatrix mat);
	SequenceComparator(shared_ptr<NonlinearFunctor> f, SimilarityMatrix mat);

	void setFunctor(shared_ptr<NonlinearFunctor> f);
	void setMatrix(SimilarityMatrix mat);
	CompareResult compare(string &sA, string &sB, string &sC);

private:
	typedef struct {
		int previous;
		int value;
	} Cell;

	void createBorders(Cell *F, int aLength, int bLength, int cLength);
	CompareResult getResultFromArray(Cell *F, string &sA, string &sB, string &sC); 
	int maxElementIndex(int *tab, int count);
	int maxElementIndex(Cell *F, int i0, int i1, int i2);

	shared_ptr<NonlinearFunctor> _functor;
	SimilarityMatrix _similarity;
};

#endif
