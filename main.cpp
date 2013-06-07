/*
 * MBI projekt, semestr 13L
 *
 * Tomasz Figa, Adam Stelmaszczyk, Piotr Takiel
 *
 * Badanie podobienstwa trzech sekwencji z nieliniowa kara za przesuniecie
 * sekwencji wzgledem siebie.
 */

#include <cstdio>
#include <cstring>
#include <iostream>
#include "SequenceComparator.h"
#include "SimilarityMatrix.h"

using namespace std;

/**
 * A quadratic non-linear function.
 * Returns minus sum of squares of all offsets.
 */
class QuadraticFunctor : public NonlinearFunctor {
public:
	virtual int value(const int a, const int b, const int c) {
		return -(a*a + b*b + c*c);
	}
};

/** Translation table for DNA. */
static const char DNATranslation[5] = {
	'a', 'c', 'g', 't', '-'
};

/** Translation table for RNA. */
static const char RNATranslation[5] = {
	'a', 'c', 'g', 'u', '-'
};

/**
 * Converts a string of ASCII characters representing input sequence
 * to internal alphabet and determines translation table.
 * @param str Input sequence stored using internal alphabet.
 * @param input Input sequence stored as a string.
 * @param translationTable Pointer pointing where to store selected translation
 * Table. If translation table could not be determined, NULL is stored.
 * @return 0 if input data was correct, negative error code otherwise.
 */
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

/**
 * Converts a sequence from internal representation to a string of ASCII
 * characters using given translation table.
 * @param str Output sequence stored as a string of ASCII characters.
 * @param output Input sequence stored in internal representation.
 * @param translationTable Translation table to use for conversion.
 */
static void translateOutput(string &str, const vector<Alphabet> &output,
						const char *translationTable) {
	str.clear();
	str.reserve(output.size());

	for (Alphabet a : output)
		str += translationTable[a];
}

/**
 * Tries to load custom similarity matrix from file specified in command line
 * @param argc Command line argument count (without application file name)
 * @param argv Array of command line argument (without application file name)
 * @return Pointer to similarity matrix, either simple one or custom loaded
 * from file. NULL on file read error.
 */
static shared_ptr<SimilarityMatrix> loadSimilarityMatrix(
						int argc, char *argv[]) {
	const char *fileName = NULL;
	int data[MAX_SYMBOL][MAX_SYMBOL];
	int pausePenalty;

	/* Look for matrix file name in command line */
	for (int i = 0; i < argc - 1; ++i, ++argv) {
		if (!strcmp(*argv, "-f"))
			fileName = *(++argv);
	}

	if (!fileName)
		return shared_ptr<SimilarityMatrix>(new SimpleSimilarityMatrix);

	/* Found matrix file name in command line */
	FILE *fMatrix = fopen(fileName, "r");
	if (!fMatrix) {
		cerr << "Failed to open matrix file" << endl;
		return NULL;
	}

	/* Load matrix data */
	for (int x = 0; x < MAX_SYMBOL; ++x)
		for (int y = 0; y < MAX_SYMBOL; ++y)
			if (fscanf(fMatrix, "%d", &data[x][y]) != 1) {
				cerr << "Failed to read matrix data" << endl;
				fclose(fMatrix);
				return NULL;
			}

	/* Load pause penalty */
	if (fscanf(fMatrix, "%d", &pausePenalty) != 1) {
		cerr << "Failed to read matrix data" << endl;
		fclose(fMatrix);
		return NULL;
	}

	fclose(fMatrix);

	return shared_ptr<SimilarityMatrix>(
			new CustomSimilarityMatrix(data, pausePenalty));
}

/**
 * Main application entry point.
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return 0 on success, negative on error.
 */
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

	shared_ptr<SimilarityMatrix> matrix =
				loadSimilarityMatrix(argc - 1, argv + 1);
	if (!matrix)
		return -2;

	SequenceComparator comparator(shared_ptr<NonlinearFunctor>(
				new QuadraticFunctor()), matrix);
	SequenceComparator::CompareResult result =
				comparator.compare(in0, in1, in2);

	translateOutput(str0, result.sA, translationTable);
	translateOutput(str1, result.sB, translationTable);
	translateOutput(str2, result.sC, translationTable);

	cout << str0 << endl;
	cout << str1 << endl;
	cout << str2 << endl;
	cout << result.value << endl;

	return 0;
}
