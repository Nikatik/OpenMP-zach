CXX=g++
CXXFLAGS=-c -g -std=c++11 -O2
TARGETS = main.o

all: finish

finish: main.o
	$(CXX) main.o -o zach2.out
	$> make clean

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp


clean:
	rm $(TARGETS)
