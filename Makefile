all:
	g++ main.cpp SequenceComparator.cpp SimilarityMatrix.cpp -O2 -o mbi

clean:
	rm -f mbi	
