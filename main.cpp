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

static const char DNATranslation[5] = {
	'a', 'c', 'g', 't', '-'
};

static const char RNATranslation[5] = {
	'a', 'c', 'g', 'u', '-'
};

static int validateInput(vector<Alphabet> &str, const string &input,
						const char **translationTable) {
	bool uFound = false;
	bool tFound = false;

	str.clear();
	str.reserve(input.size());

	for (char a : input) {
		switch (a) {
		case 'A':
		case 'a':
			str.push_back(SYMBOL_A);
			break;
		case 'C':
		case 'c':
			str.push_back(SYMBOL_C);
			break;
		case 'G':
		case 'g':
			str.push_back(SYMBOL_G);
			break;
		case 'U':
		case 'u':
			uFound = true;
			if (tFound)
				return -EINVAL;
			str.push_back(SYMBOL_UT);
			break;
		case 'T':
		case 't':
			tFound = true;
			if (uFound)
				return -EINVAL;
			str.push_back(SYMBOL_UT);
			break;
		default:
			return -EINVAL;
		}
	}

	if (tFound) {
		if (*translationTable && *translationTable != DNATranslation)
			return -EINVAL;
		*translationTable = DNATranslation;
	} else if (uFound) {
		if (*translationTable && *translationTable != RNATranslation)
			return -EINVAL;
		*translationTable = RNATranslation;
	}

	return 0;
}

static void translateOutput(string &str, const vector<Alphabet> &output,
						const char *translationTable) {
	str.clear();
	str.reserve(output.size());

	for (char a : output)
		str += translationTable[a];
}

int main(int argc, char *argv[]) {
	string str0, str1, str2;
	vector<Alphabet> in0, in1, in2;
	const char *translationTable = NULL;

	cin >> str0;
	cin >> str1;
	cin >> str2;

	if (validateInput(in0, str0, &translationTable)) {
		cerr << "First sequence is invalid" << endl;
		return -1;
	}

	if (validateInput(in1, str1, &translationTable)) {
		cerr << "Second sequence is invalid" << endl;
		return -1;
	}

	if (validateInput(in2, str2, &translationTable)) {
		cerr << "Third sequence is invalid" << endl;
		return -1;
	}

	if (!translationTable)
		translationTable = DNATranslation;

	SequenceComparator comparator(shared_ptr<NonlinearFunctor>(new QuadraticFunctor()));
	SequenceComparator::CompareResult result = comparator.compare(in0, in1, in2);

	translateOutput(str0, result.sA, translationTable);
	translateOutput(str1, result.sB, translationTable);
	translateOutput(str2, result.sC, translationTable);

	cout << str0 << endl;
	cout << str1 << endl;
	cout << str2 << endl;
	cout << result.value << endl;

	return 0;
}
