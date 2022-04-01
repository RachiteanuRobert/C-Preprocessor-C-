typedef struct arraylist{
    int no_items;       
    int size;           
    void** content;
} arraylist;

int new_arraylist(arraylist** alist, int start_size);
int add_elem_arraylist(arraylist* alist, void* element);
void show_arraylist(arraylist* alist);
void free_arraylist(arraylist* alist);