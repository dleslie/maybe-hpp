all: maybe.hpp test.cpp
	gcc -std=c++11 test.cpp -lstdc++ -o test

%.hpp:

%.cpp:
