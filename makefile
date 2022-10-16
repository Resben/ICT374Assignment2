#makefile for main

main: main.o ./src/token.o ./src/prompt.o ./src/cd.o
	gcc main.o ./src/token.o ./src/prompt.o ./src/cd.o -o main

main.o: main.c ./include/token.h ./include/prompt.h
	gcc -c main.c

token.o: ./src/token.c ./include/token.h
	gcc -c ./src/token.c

prompt.o: ./src/prompt.c ./include/prompt.h
	gcc -c ./src/prompt.c

cd.o: ./src/cd.c ./include/cd.h
	gcc -c ./src/cd.c

clean: 
	rm *.o

