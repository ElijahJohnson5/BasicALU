CC=gcc
CFLAGS=-g -Wall

main: operations.o print.o registry.o init.o main.c
	$(CC) $(CFLAGS) init.o main.c operations.o registry.o print.o -o calculator

init.o: init.c
	$(CC) $(CFLAGS) -c init.c

operations.o: operations.c
	$(CC) $(CFLAGS) -c operations.c

print.o: print.c
	$(CC) $(CFLAGS) -c print.c

registry.o: registry.c
	$(CC) $(CFLAGS) -c registry.c

clean: 
	rm init.o registry.o print.o operations.o
