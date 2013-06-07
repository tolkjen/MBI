/*
 * MBI projekt, semestr 13L
 *
 * Tomasz Figa, Adam Stelmaszczyk, Piotr Takiel
 *
 * Badanie podobienstwa trzech sekwencji z nieliniowa kara za przesuniecie
 * sekwencji wzgledem siebie.
 */

#ifndef SEQUENCE_COMPARATOR_H
#define SEQUENCE_COMPARATOR_H

#include <string>
#include <vector>
#include <memory>
#include "NonlinearFunctor.h"
#include "SimilarityMatrix.h"

using namespace std;

class SequenceComparator
{
public:
	typedef struct {
		int value;
		vector<Alphabet> sA;
		vector<Alphabet> sB;
		vector<Alphabet> sC;
	} CompareResult;

	/*
	 * Creates a comparator object with a default similatity matrix and no 
	 * nonlinear functor. Before using compare(), set a functor by setFunctor().
	 */
	SequenceComparator(void);
	
	/*
	 * Creates a comparator object with a default similatity matrix and a 
	 * functor passed as a parameter.
	 * 
	 * @param f Nonlinear functor object.
	 */
	SequenceComparator(shared_ptr<NonlinearFunctor> f);
	
	/*
	 * Creates a comparator object with a similarity matrix passed as a 
	 * parameter and no functor. Before using compare(), set a functor by 
	 * setFunctor().
	 * 
	 * @param mat Similarity matrix object.
	 */
	SequenceComparator(const shared_ptr<SimilarityMatrix> mat);
	
	/*
	 * Creates a comparator object with a similarity matrix passed as a 
	 * parameter and a functor passed as a parameter.
	 * 
	 * @param f Nonlinear functor object.
	 * @param mat Similarity matrix object.
	 */
	SequenceComparator(shared_ptr<NonlinearFunctor> f,
					const shared_ptr<SimilarityMatrix> mat);

	/*
	 * Sets object's functor to the one passed as a parameter.
	 * 
	 * @param f Nonlinear functor object.
	 */
	void setFunctor(shared_ptr<NonlinearFunctor> f);
	
	/*
	 * Sets object's similarity matrix to the one passed as a parameter.
	 * 
	 * @param mat Similarity matrix object.
	 */
	void setMatrix(const shared_ptr<SimilarityMatrix> mat);
	
	/*
	 * Compares three strings and returns a struct containing the match value 
	 * and matched strings.
	 * 
	 * @param sA First string.
	 * @param sB Second string.
	 * @param sC Third string
	 * @return A struct witch match value and matched strings.
	 */
	CompareResult compare(const vector<Alphabet> &sA,
			const vector<Alphabet> &sB, const vector<Alphabet> &sC);

private:
	typedef struct {
		int previous;
		int value;
	} Cell;

	void createBorders(Cell *F, int aLength, int bLength, int cLength);
	CompareResult getResultFromArray(Cell *F, const vector<Alphabet> &sA,
			const vector<Alphabet> &sB, const vector<Alphabet> &sC);
	int maxElementIndex(int *tab, int count);
	int maxElementIndex(Cell *F, int i0, int i1, int i2);

	shared_ptr<NonlinearFunctor> _functor;
	shared_ptr<SimilarityMatrix> _similarity;
};

#endif
