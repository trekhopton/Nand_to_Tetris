#a1696375

# must specify make target
all: tokeniser-m tokeniser-f

# C++ 11
CXXFLAGS=--std=c++11

MYCPP:=${wildcard my*.cpp}
MYO:=${MYCPP:.cpp=.o}

tokeniser-m: mainm.o tokensm.o ${MYO} libstcx.a
	g++ -o tokeniser-m ${CXXFLAGS} mainm.o tokensm.o ${MYO} libstcx.a

tokeniser-f: mainf.o tokensf.o ${MYO} libstcx.a
	g++ -o tokeniser-f ${CXXFLAGS} mainf.o tokensf.o ${MYO} libstcx.a

clean:
	rm -f tokeniser-m tokeniser-f *.o
