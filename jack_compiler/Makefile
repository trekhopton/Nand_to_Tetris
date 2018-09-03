# all the executables
EXECS= jackparser jackpretty jacksymbols jackvmcode
MYALL:=${wildcard myall_*.cpp}
MYPARSER:=${wildcard myparser_*.cpp}
MYPRETTY:=${wildcard mypretty_*.cpp}
MYSYMBOLS:=${wildcard mysymbols_*.cpp}
MYVMCODE:=${wildcard myvmcode_*.cpp}

MYALLO:=${MYALL:.cpp=.o}
MYPARSERO:=${MYPARSER:.cpp=.o}
MYPRETTYO:=${MYPRETTY:.cpp=.o}
MYSYMBOLSO:=${MYSYMBOLS:.cpp=.o}
MYVMCODEO:=${MYVMCODE:.cpp=.o}

# C++ 11 just in case
CXXFLAGS=--std=c++11

# must specify make target
all: ${EXECS}

clean:
	rm -f ${EXECS} *.o

%.o: %.cpp
	-(g++ ${CXXFLAGS} -c $<)

# parser
jackparser: ${MYPARSERO} ${MYALLO} libxst.a
	-(g++ ${CXXFLAGS} -o $@ ${MYPARSERO} ${MYALLO} libxst.a)

# prettys
jackpretty: ${MYPRETTYO} ${MYALLO} libxst.a
	-(g++ ${CXXFLAGS} -o $@ ${MYPRETTYO} ${MYALLO} libxst.a)

# symbols
jacksymbols: ${MYSYMBOLSO} ${MYALLO} libxst.a
	-(g++ ${CXXFLAGS} -o $@ ${MYSYMBOLSO} ${MYALLO} libxst.a)

# code
jackvmcode: ${MYVMCODEO} ${MYALLO} libxst.a
	-(g++ ${CXXFLAGS} -o $@ ${MYVMCODEO} ${MYALLO} libxst.a)


