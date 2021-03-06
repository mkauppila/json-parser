main: main.o 
	$(CC) main.o -o main

test: test.o 
	$(CC) test.o -o test

main.o: main.c
	$(CC) -c main.c -o main.o

test.o: test.c
	$(CC) -c test.c -o test.o

clean:
	rm test 2> /dev/null
	rm *.o 2> /dev/null
