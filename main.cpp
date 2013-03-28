#include <iostream>
#include "SequenceComparator.h"

using namespace std;

class QuadraticFunctor : public NonlinearFunctor {
public:
	virtual int value(const int a, const int b, const int c) {
		return -(a*a + b*b + c*c);
	}
};

int main() {
	SequenceComparator comparator(shared_ptr<NonlinearFunctor>(new QuadraticFunctor()));

	string str0 = "kukurydza";
	string str1 = "kurtyna";
	string str2 = "rydzyk";
	auto result = comparator.compare(str0, str1, str2);
	cout << result.value << endl;
	cout << result.sA.c_str() << endl << result.sB.c_str() << endl << result.sC.c_str();

	getchar();
	return 0;
}
