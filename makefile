programa: main.c colas.o btree.o tablahash.o wstring.o sugerencias.o
	gcc -o programa main.c colas.o btree.o tablahash.o wstring.o sugerencias.o -Wall -pedantic
btree.o: btree.c btree.h
	gcc -c btree.c -Wall -pedantic
colas.o: colas.c colas.h
	gcc -c colas.c -Wall -pedantic
tablahash.o: tablahash.c tablahash.h
	gcc -c tablahash.c -Wall -pedantic
wstring.o: wstring.c wstring.h
	gcc -c wstring.c -Wall -pedantic
sugerencias.o: sugerencias.c sugerencias.h
	gcc -c sugerencias.c -Wall -pedantic
clean:
	rm -f programa *.o
