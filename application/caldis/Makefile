cc = g++
OP = -O3 -march=native
WR = -Wall -Wextra
ST = -std=c++17 

all: 2calcBm 3calcInfall 4infall2chemComp

2calcBm: 2calcBm.cc ../../bmExpansion.o
	$(CC) $(OP) $(WR) $(ST) -lm -lstdc++ ../../bmExpansion.o 2calcBm.cc -o2calcBm

3calcInfall: 3calcInfall.cc
	$(CC) $(OP) $(WR) $(ST) -lm -lstdc++ 3calcInfall.cc -o3calcInfall

4infall2chemComp: 4infall2chemComp.cc
	$(CC) $(OP) $(WR) $(ST) -lm -lstdc++ 4infall2chemComp.cc -o4infall2chemComp
