#include <iostream>
#include "SequenceComparator.h"
#include "SimilarityMatrix.h"

using namespace std;

class QuadraticFunctor : public NonlinearFunctor {
public:
	virtual int value(const int a, const int b, const int c) {
		return -(a*a + b*b + c*c);
	}
};

int main() {
	SequenceComparator comparator(shared_ptr<NonlinearFunctor>(new QuadraticFunctor()));

	// Przyk³ad z wyk³adu 5, slajd 20
	string str0 = "agttat";
	string str1 = "gtcgtt";
	string str2 = "attcgtat";
	auto result = comparator.compare(str0, str1, str2);

	// Wydruk wyniku
	cout << "Comparison value: " << result.value << endl << endl;
	cout << result.sA.c_str() << endl << result.sB.c_str() << endl << result.sC.c_str() << endl;

	getchar();
	return 0;
}
