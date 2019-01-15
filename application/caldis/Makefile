CC = g++
OP = -O3 -march=native
WR = -Wall -Wextra
ST = -std=c++17 

all: 1_5sdiMax 2calcBm 3calcInfall 4infall2chemComp 5threshold t1calcStock

1_5sdiMax: 1_5sdiMax.cc
	$(CC) $(OP) $(WR) $(ST) -lm -lstdc++ 1_5sdiMax.cc -o1_5sdiMax

2calcBm: 2calcBm.cc ../../bmExpansion.o
	$(CC) $(OP) $(WR) $(ST) -lm -lstdc++ ../../bmExpansion.o 2calcBm.cc -o2calcBm

3calcInfall: 3calcInfall.cc
	$(CC) $(OP) $(WR) $(ST) -lm -lstdc++ 3calcInfall.cc -o3calcInfall

4infall2chemComp: 4infall2chemComp.cc
	$(CC) $(OP) $(WR) $(ST) -lm -lstdc++ 4infall2chemComp.cc -o4infall2chemComp

5threshold: 5threshold.cc
	$(CC) $(OP) $(WR) $(ST) -lm -lstdc++ 5threshold.cc -o5threshold

t1calcStock: t1calcStock.cc
	$(CC) $(OP) $(WR) $(ST) -lm -lstdc++ t1calcStock.cc -ot1calcStock
