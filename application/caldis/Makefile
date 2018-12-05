cc = g++
OP = -O3 -march=native
WR = -Wall -Wextra
ST = -std=c++17 

all: 2calcBm

2calcBm: 2calcBm.cc ../../bmExpansion.o
	$(CC) $(OP) $(WR) $(ST) -lm -lstdc++ ../../bmExpansion.o 2calcBm.cc -o2calcBm
