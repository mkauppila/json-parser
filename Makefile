main: main.o json.o
	$(CC) json.o main.o -o main

test: test.o json.o
	$(CC) test.o json.o -o test

main.o: main.c
	$(CC) -c main.c -o main.o

test.o: test.c
	$(CC) -c test.c -o test.o

json.o: json.c
	$(CC) -c json.c -o json.o

clean:
	rm test 2> /dev/null
	rm *.o 2> /dev/null
