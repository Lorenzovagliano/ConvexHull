CC=g++
CFLAGS=-std=c++11 -Wall

TARGET= ./bin/PilhaFila
OBJ_DIR = ./obj
SRC_DIR = ./src
INCLUDE_DIR = ./include

all: ${TARGET}

${TARGET}: ${OBJ_DIR}/FilaCircular.o ${OBJ_DIR}/Pilha.o ${OBJ_DIR}/main.o
	${CC} ${CFLAGS} ${OBJ_DIR}/*.o -o ${TARGET}

${OBJ_DIR}/FilaCircular.o: ${INCLUDE_DIR}/FilaCircular.hpp ${SRC_DIR}/FilaCircular.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/FilaCircular.cpp -o ${OBJ_DIR}/FilaCircular.o

${OBJ_DIR}/Pilha.o: ${INCLUDE_DIR}/FilaCircular.hpp ${INCLUDE_DIR}/Pilha.hpp ${SRC_DIR}/Pilha.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/Pilha.cpp -o ${OBJ_DIR}/Pilha.o

${OBJ_DIR}/main.o: ${INCLUDE_DIR}/FilaCircular.hpp ${INCLUDE_DIR}/Pilha.hpp ${SRC_DIR}/main.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/main.cpp -o ${OBJ_DIR}/main.o

run: all
	${TARGET} ./PilhaFila

clean:
	rm -f ${OBJ_DIR}/* ${TARGET}

