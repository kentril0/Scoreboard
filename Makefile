##############
# file: Makefile
# date: 25.8.2018
# author: Kentril Despair
# version: 1.0
# brief:
##############

CPPFLAGS=-std=c++14 -pedantic -Wall -Wextra -Werror
CXX=g++

PROJECT=scoreboard
HEADER=scoreboard.h
SOURCE=scoreboard.cc
OBJECTS=scoreboard.o main.o

all: ${PROJECT}

${PROJECT}: ${OBJECTS}
	${CXX} ${CPPFLAGS} ${OBJECTS} -o $@

scoreboard.o: ${SOURCE} ${HEADER}
	${CXX} ${CPPFLAGS} $< -c

main.o: main.cc ${HEADER}
	${CXX} ${CPPFLAGS} $< -c


clean:
	rm -f *.o ${PROJECT}
