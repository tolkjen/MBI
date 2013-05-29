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

//  --------------------------------------------------------------------------
//	Constructor (1)
//	
//	Creates a comparator object with a default similatity matrix and no 
//	nonlinear functor. Before using compare(), set a functor by setFunctor().
	SequenceComparator(void);
	
//  --------------------------------------------------------------------------
//	Constructor (2)
//	
//	Creates a comparator object with a default similatity matrix and a functor 
//	passed as a parameter.
	SequenceComparator(shared_ptr<NonlinearFunctor> f);
	
//	--------------------------------------------------------------------------
//	Constructor (3)
//	
//	Creates a comparator object with a similarity matrix passed as a parameter 
//	and no functor. Before using compare(), set a functor by setFunctor().
	SequenceComparator(SimilarityMatrix mat);
	
//	--------------------------------------------------------------------------
//	Constructor (4)
//	
//	Creates a comparator object with a similarity matrix passed as a parameter 
//	and a functor passed as a parameter.
	SequenceComparator(shared_ptr<NonlinearFunctor> f, SimilarityMatrix mat);

//	--------------------------------------------------------------------------
//	setFunctor
//	
//	Sets object's functor to the one passed as a parameter.
	void setFunctor(shared_ptr<NonlinearFunctor> f);
	
//	--------------------------------------------------------------------------
//	setMatrix
//	
//	Sets object's similarity matrix to the one passed as a parameter.
	void setMatrix(SimilarityMatrix mat);
	
//	--------------------------------------------------------------------------
//	compare
//
//	Compares three strings and returns a struct containing the match value and 
//	matched strings.
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
