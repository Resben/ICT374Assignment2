#makefile for main

main: main.o ./src/command.o ./src/token.o ./src/prompt.o
	gcc main.o ./src/command.o ./src/token.o ./src/prompt.o -o main

main.o: main.c ./include/command.h ./include/token.h ./include/prompt.h
	gcc -c main.c

command.o: ./src/command.c ./include/command.h
	gcc -c ./src/command.c

token.o: ./src/token.c ./include/token.h
	gcc -c ./src/token.c

prompt.o: ./src/prompt.c ./include/prompt.h
	gcc -c ./src/prompt.c

clean: 
	rm *.o

