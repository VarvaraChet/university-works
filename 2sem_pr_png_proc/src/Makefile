all: main.o ./src/arguments.o ./src/picture.o ./src/drawing.o ./src/process.o ./src/help.o
	gcc main.o ./src/arguments.o ./src/picture.o ./src/drawing.o ./src/process.o ./src/help.o -lpng -lm -o cw

main.o: main.c ./src/arguments.h ./src/errors.h ./src/picture.h ./src/drawing.h ./src/process.h ./src/help.h
	gcc -c main.c

arguments.o: ./src/arguments.c ./src/arguments.h ./src/errors.h
	gcc -c ./src/arguments.c

picture.o: ./src/picture.c ./src/picture.h ./src/errors.h
	gcc -c ./src/picture.c

drawing.o: ./src/drawing.c ./src/drawing.h ./src/picture.h ./src/arguments.h ./src/errors.h
	gcc -c ./src/drawing.c

process.o: ./src/process.c ./src/process.h ./src/picture.h ./src/arguments.h ./src/errors.h
	gcc -c ./src/process.c

help.o: ./src/help.c ./src/help.h
	gcc -c ./src/help.c

clean:
	rm -rf *.o
