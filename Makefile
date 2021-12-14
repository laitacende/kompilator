# Makefile

FLAGS = -W -pedantic -std=c++17 -O3

.PHONY = all clean cleanall

all: kompilator

kompilator: lex parser
	g++ $(FLAGS) -o kompilator -I ./inc ./src/*.cpp parser.tab.cpp lex.yy.c


lex:
	flex lex.l

parser:
	bison -Wall -d parser.ypp


clean:
	rm -f *.o *.cc *.hh *.cpp

cleanall: clean
	rm -f kompilator