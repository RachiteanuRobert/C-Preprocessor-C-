#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "array_list.h"
#include "hash_table.h"


//Creates new arraylist with given start size
int new_arraylist(arraylist** alist, int start_size){
    arraylist* new_arraylist = calloc(1, sizeof(arraylist));
    if(new_arraylist == NULL)
        return 12;
    new_arraylist->no_items = 0;
    new_arraylist->size = start_size;
    new_arraylist->content = calloc(1, sizeof(void*) * start_size);
    if(new_arraylist == NULL)
        return 12;
    *alist = new_arraylist;
    
    return 0;
}

//Adds a new element to the arraylist
int add_elem_arraylist(arraylist* alist, void* element){
    int new_size;
    int added_no_items = alist->no_items + 1;

    alist->no_items++;

    //If arraylist doesn't have enough capacity, double it
    if(added_no_items > alist->size){
        new_size = alist->size;
        while(new_size < added_no_items)
            new_size *= 2;
        alist->content = realloc(alist->content, sizeof(void*) * new_size);
        if(alist->content == NULL)
            return 12;
        alist->size = new_size;
    }
    alist->content[alist->no_items - 1] = element;

    return 0;
}

//Print arraylist
void show_arraylist(arraylist* alist){
    int i;

    for(i = 0; i < alist->no_items; i++){
        printf("[%d]: k-%s- v-%s-\n", i, ((hashentry*)alist->content[i])->key, 
            ((hashentry*)alist->content[i])->value);
    }
}

void free_arraylist(arraylist* alist){
    int i;

    for(i = 0; i < alist->no_items; i++){
        free_hashentry((hashentry*)alist->content[i]);
    }
    free(alist->content);
    free(alist);
}
