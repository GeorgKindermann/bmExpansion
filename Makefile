cc = g++
OP = -O3 -march=native
WR = -Wall -Wextra
ST = -std=c++17 

all: bmExpansion.o testBmExpansion

bmExpansion.o: bmExpansion.h bmExpansion.cc
	$(CC) $(OP) $(WR) $(ST) -c bmExpansion.cc

testBmExpansion: testBmExpansion.cc bmExpansion.o
	$(CC) $(OP) $(WR) $(ST) -lm -lstdc++ bmExpansion.o testBmExpansion.cc -otestBmExpansion
	strip testBmExpansion
