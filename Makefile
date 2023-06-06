CC=g++
CFLAGS=-std=c++11 -Wall

TARGET= ./bin/ConvexHull
OBJ_DIR = ./obj
SRC_DIR = ./src
INCLUDE_DIR = ./include

all: ${TARGET}

${TARGET}: ${OBJ_DIR}/Ponto.o ${OBJ_DIR}/Reta.o ${OBJ_DIR}/main.o
	${CC} ${CFLAGS} ${OBJ_DIR}/*.o -o ${TARGET}

${OBJ_DIR}/Ponto.o: ${INCLUDE_DIR}/Ponto.hpp ${SRC_DIR}/Ponto.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/Ponto.cpp -o ${OBJ_DIR}/Ponto.o

${OBJ_DIR}/Reta.o: ${INCLUDE_DIR}/Ponto.hpp ${INCLUDE_DIR}/Reta.hpp ${SRC_DIR}/Reta.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/Reta.cpp -o ${OBJ_DIR}/Reta.o

${OBJ_DIR}/main.o: ${INCLUDE_DIR}/Ponto.hpp ${INCLUDE_DIR}/Reta.hpp ${SRC_DIR}/main.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/main.cpp -o ${OBJ_DIR}/main.o

run: all
	${TARGET} $(ENTRADA)

clean:
	rm -f ${OBJ_DIR}/* ${TARGET}

