typedef struct hashentry{
    char *key;
    char *value;
} hashentry;

typedef struct hashtable{
    int no_items;       
    int size;           
    void** content;
} hashtable;

typedef enum { false, true } bool;

typedef enum { D, I, o, infile } cmd_type;

unsigned long hash(char *key);
int new_hashtable(hashtable **htable, int start_size);
int new_hashentry(hashentry **hentry,char *key, char *value);
bool key_exists_hashtable(hashtable* htable, char* key);
char* find_key_value_hashtable(hashtable* htable, char* key, int and_destroy);
int add_elem_hashtable(hashtable* htable, char* key, char* value);
void show_hashtable(hashtable* htable);
void free_hashentry(hashentry* hentry);
void free_hashtable(hashtable* htable);