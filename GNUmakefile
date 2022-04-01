build: tema1.o hash_table.o array_list.o
	gcc tema1.o hash_table.o array_list.o -Wall -g -o checker/multi/so-cpp 

tema1.o: tema1.c
	gcc -g -c tema1.c

hash_table.o: hash_table.c hash_table.h
	gcc -g -c hash_table.c

array_list.o: array_list.c array_list.h
	gcc -g -c array_list.c

clean:
	rm *.o so-cpp
