#a1696375
# must specify make target
all: assembler-m assembler-f

# C++ 11
CXXFLAGS=--std=c++11

clean:
	rm -rf assembler-m assembler-f *.o

MYCPP:=${wildcard my*.cpp}
MYO:=${MYCPP:.cpp=.o}

assembler-m: mainm.o ${MYO} libstcx.a
	g++ -o assembler-m ${CXXFLAGS} mainm.o ${MYO} libstcx.a

assembler-f: mainf.o ${MYO} libstcx.a
	g++ -o assembler-f ${CXXFLAGS} mainf.o ${MYO} libstcx.a
