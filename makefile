#makefile for main

main: main.o ./src/token.o
	gcc main.o ./src/token.o -o main

main.o: main.c ./include/token.h
	gcc -c main.c

token.o: ./src/token.c ./include/token.h
	gcc -c ./src/token.c

clean: 
	rm *.o

