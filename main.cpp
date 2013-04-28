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

int main(int argc, char *argv[]) {
	string str0, str1, str2;
	cin >> str0;
	cin >> str1;
	cin >> str2;

	SequenceComparator comparator(shared_ptr<NonlinearFunctor>(new QuadraticFunctor()));
	SequenceComparator::CompareResult result = comparator.compare(str0, str1, str2);

	cout << result.sA << endl;
	cout << result.sB << endl;
	cout << result.sC << endl;
	cout << result.value << endl;

	return 0;
}
