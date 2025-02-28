CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -Iinclude -Iinclude/catch2

all: test

longnum.o: longnum.cpp longnum.hpp
	$(CXX) $(CXXFLAGS) -c longnum.cpp -o longnum.o

catch_amalgamated.o: include/catch2/catch_amalgamated.cpp include/catch2/catch_amalgamated.hpp
	$(CXX) $(CXXFLAGS) -c include/catch2/catch_amalgamated.cpp -o catch_amalgamated.o

test_longnum.o: test_longnum.cpp longnum.hpp
	$(CXX) $(CXXFLAGS) -c test_longnum.cpp -o test_longnum.o

test: test_longnum.o longnum.o catch_amalgamated.o
	$(CXX) $(CXXFLAGS) test_longnum.o longnum.o catch_amalgamated.o -o test_longnum
	./test_longnum

clean:
	rm -f *.o test_longnum
