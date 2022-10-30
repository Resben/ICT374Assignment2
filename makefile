#makefile for main

main: main.o ./src/command.o ./src/token.o ./src/prompt.o ./src/placeholder.o ./src/cd.o
	gcc main.o ./src/command.o ./src/token.o ./src/placeholder.o ./src/cd.o ./src/prompt.o -o main

main.o: main.c ./include/command.h ./include/token.h ./include/prompt.h ./include/placeholder.h ./include/cd.h
	gcc -c main.c

command.o: ./src/command.c ./include/command.h
	gcc -c ./src/command.c

token.o: ./src/token.c ./include/token.h
	gcc -c ./src/token.c

placeholder.o: ./src/placeholder.c ./include/placeholder.h
	gcc -c ./src/placeholder.c

cd.o: ./src/cd.c ./include/cd.h
	gcc -c ./src/cd.c

prompt.o: ./src/prompt.c ./include/prompt.h
	gcc -c ./src/prompt.c

clean:
	rm *.o

