CXX = g++
longnum.o:
  $(CXX) longnum.cpp -c
longnum.a: longnum.o
  $(CXX) longnum.o -o longnum.a