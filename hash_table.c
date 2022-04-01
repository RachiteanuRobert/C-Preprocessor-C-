#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash_table.h"
#include "array_list.h"

#define ARRAYLIST_START_SIZE 5

//Hash function
unsigned long hash(char *key){
    unsigned long hash = 5381;
    int c;
    while (c = *key++)
        hash = ((hash << 5) + hash) + c; 

    return hash;
}

//Creates new hashtable with given start size
int new_hashtable(hashtable **htable, int start_size){
    hashtable* new_hashtable = calloc(1, sizeof(hashtable));
    if(new_hashentry == NULL)
        return 12;
    new_hashtable->no_items = 0;
    new_hashtable->size = start_size;
    new_hashtable->content = calloc(start_size, sizeof(void*));
    if(new_hashtable->content == NULL)
        return 12;
    *htable = new_hashtable;
    
    return 0;
}

//Creates new hashentry with given start size
int new_hashentry(hashentry **hentry, char *key, char *value){
    hashentry* new_hashentry = calloc(1, sizeof(hashentry));
    if(new_hashentry == NULL)
        return 12;
    new_hashentry->key = calloc(100, sizeof(char));
    if(new_hashentry->key == NULL)
        return 12;
    new_hashentry->value = calloc(100, sizeof(char));
    if(new_hashentry->value == NULL)
        return 12;
    strcpy(new_hashentry->key, key);
    strcpy(new_hashentry->value, value);
    *hentry = new_hashentry;

    return 0;
}

//Check if given key is in hashtable
int key_exists_hashtable(hashtable* htable, char* key){
    if(htable->content[hash(key) % htable->size] != NULL)
        return 1;
    else
        return 0;
}

//Returns the value of the given key from hashtable or the key itself if
//it does not exist
char *find_key_value_hashtable(hashtable* htable, char* key, int and_destroy){
    int i;
    int index = hash(key) % htable->size;
    
    if(key_exists_hashtable(htable, key)){
        arraylist* hash_alist = htable->content[index];

        //Iterate through hashtable slot arraylist 
        for(i = 0; i < hash_alist->no_items; i++){
            hashentry* curr_hashentry = hash_alist->content[i];
            if(strcmp(curr_hashentry->key, key) == 0){
                if(and_destroy){
                    strncpy(((hashentry*)hash_alist->content[i])->key, "",
                        sizeof(((hashentry*)hash_alist->content[i])->key));
                }
                else{
                    return curr_hashentry->value;
                }
            }
        }
    }
    
    return key;
}

//Adds new hashentry to hashtable with given key and value
int add_elem_hashtable(hashtable* htable, char* key, char* value){
    arraylist *alist;
    hashentry* new_entry;
    int ret_val;
    int index = hash(key) % htable->size;

    if(new_hashentry(&new_entry, key, value) == 12)
        return 12;
    if(htable->content[index] == NULL){
        ret_val = new_arraylist((arraylist**)(&(htable->content[index])),
            ARRAYLIST_START_SIZE); 
        if(ret_val == 12)
            return 12; 
    }
    
    ret_val = add_elem_arraylist((arraylist*)htable->content[index],
        new_entry);
    if(ret_val == 12)
        return 12;
    htable->no_items++;

    return 0;
}

//Prints hashtable
void show_hashtable(hashtable* htable){
    int i;
    
    printf("Hash Table[%d]:\n", htable->no_items);
    for(i = 0; i < htable->size - 1; i++){
        if(htable->content[i] != NULL){
            show_arraylist((arraylist*)htable->content[i]);
        }
    }
}

void free_hashentry(hashentry* hentry){
    free(hentry->key);
    free(hentry->value);
    free(hentry);
}

void free_hashtable(hashtable* htable){
    int i;

    for(i = 0; i < htable->size; i++){
        if(htable->content[i] != NULL){
            free_arraylist((arraylist*)htable->content[i]);
        }
    }
    free(htable->content);
    free(htable);
}

