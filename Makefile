CC=g++
CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

TARGET= ./bin/ConvexHull
OBJ_DIR = ./obj
SRC_DIR = ./src
INCLUDE_DIR = ./include

all: ${TARGET}

${TARGET}: ${OBJ_DIR}/Ponto.o ${OBJ_DIR}/Reta.o ${OBJ_DIR}/Fecho.o ${OBJ_DIR}/main.o
	${CC} ${OBJ_DIR}/*.o -o ${TARGET} ${CFLAGS}

${OBJ_DIR}/Ponto.o: ${INCLUDE_DIR}/Ponto.hpp ${SRC_DIR}/Ponto.cpp
	${CC} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/Ponto.cpp -o ${OBJ_DIR}/Ponto.o ${CFLAGS}

${OBJ_DIR}/Reta.o: ${INCLUDE_DIR}/Ponto.hpp ${INCLUDE_DIR}/Reta.hpp ${SRC_DIR}/Reta.cpp
	${CC} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/Reta.cpp -o ${OBJ_DIR}/Reta.o ${CFLAGS}

${OBJ_DIR}/Fecho.o: ${INCLUDE_DIR}/Reta.hpp ${INCLUDE_DIR}/Reta.hpp ${INCLUDE_DIR}/Fecho.hpp ${SRC_DIR}/Fecho.cpp
	${CC} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/Fecho.cpp -o ${OBJ_DIR}/Fecho.o ${CFLAGS}

${OBJ_DIR}/main.o: ${INCLUDE_DIR}/Ponto.hpp ${INCLUDE_DIR}/Reta.hpp ${INCLUDE_DIR}/Fecho.hpp ${SRC_DIR}/main.cpp
	${CC} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/main.cpp -o ${OBJ_DIR}/main.o ${CFLAGS}

install:
	sudo apt-get install libsfml-dev
	
run: all
	${TARGET} $(ENTRADA) $(VIDEO)

clean:
	rm -f ${OBJ_DIR}/* ${TARGET}
