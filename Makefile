#########################
# file: Makefile
# date: 25.8.2018
# author: Kentril Despair
# version: 1.0
# brief: TODO
########################

CPPFLAGS=-std=c++17 -pedantic -Wall -Wextra -Werror -D DEBUG
CXX=g++

# scoreboard project
PROJECT=scoreboard
HEADER=scoreboard.h
SOURCE=scoreboard.cc

# interface
INTFC_S=interface.cc
INTFC_H=interface.h

OBJECTS=scoreboard.o interface.o main.o

# -------------------------------------------------------------------------
# main label
all: ${PROJECT}

${PROJECT}: ${OBJECTS}
	${CXX} ${CPPFLAGS} ${OBJECTS} -o $@

scoreboard.o: ${SOURCE} ${HEADER}
	${CXX} ${CPPFLAGS} $< -c

interface.o: ${INTFC_S} ${INTFC_H} ${HEADER}
	${CXX} ${CPPFLAGS} $< -c

main.o: main.cc ${INTFC_H}
	${CXX} ${CPPFLAGS} $< -c


clean:
	rm -f *.o ${PROJECT}
