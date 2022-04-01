CC = cl.exe
CFLAGS = /Wall /MD

build: so-cpp.exe

so-cpp.exe: tema1.o hash_table.o array_list.o
	$(CC) $(CFLAGS) tema1.o hash_table.o array_list.o /Feso-cpp.exe

tema1.o: tema1.c
	$(CC) $(CFLAGS) /c tema1.c /Fotema1.o

hash_table.o: hash_table.c hash_table.h
	$(CC) $(CFLAGS) /c hash_table.c /Fohash_table.o

array_list.o: array_list.c array_list.h
	$(CC) $(CFLAGS) /c array_list.c /Foarray_list.o

clean:
	del *.o so-cpp