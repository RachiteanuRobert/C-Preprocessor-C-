RACHITEANU ROBERT
332CB

TEMA 1 - SO


CONTAINED FILES:

array_list.c - functions:
new_arraylist() - allocates memory for new array list of specified size;

add_elem_arraylist() - checks if array list has enough capacity for given 
element and atributes it if it has. If it doesn't, it doubles its size and 
reallocates memory for it;

free_arraylist() - iterates through each element of array list and deallocates
it.



hash_table.c - functions:
hash() - hashing function for hash table

new_hashtable() - allocates memory for new hash table of specified size;

new_hashentry() - allocates memory for new hash entry of specified size;

key_exists_hashtable() - checks if the given key is in hash_table;

find_key_value_hashtable() - iterates through the elements of the array list at
(key % hash_table->size) and returns the value if the key matched with any of
them;

add_elem_hashtable() - checks if there is an array list at position (key %
hash_table->size) and allocates a new one in the hash table content if there
isn't. Inserts new element there.



tema1.c - functions:
check_directive() - checks if the first character which isn't a space is '#'
and returns it's position if it is else -1;

return_clear_line() - returns the given string without indentation 

main(): iterates through each argument and checks if it or the next one is a 
command ('-' is the first character). For each if the command didn't have an
expected argument it awaits for one in the next arg (cmd_needs_arg variable).
    For -D: parses symbol and mapping from current argument (or from the next 
    one) and adds them to the hash table.
    For -o: counts command occurance in no_out_files and stores the value of 
    the output file. If multiple instances occured: error (return 12).
    For -I: stores each directory in array possible_directories.
    For input: checks if there are multiple input files and if true return 12.

If there is an input file which has to be read iterates through all possible 
directories, adds the input file at the end and tries to read from it, else
reads from stdin.

Iterates from each line of the file and checks with the if, ifdef, etc logics
to see if it actually writes the next to be processed line.
Extracts each word separated from specified delimitators and checks if any of
them are keys for the hash table. If one matches it replaces them:
    -finds and stores all positions of ' " ' character;
    -iterates through string and if it finds the to be replaced word and it's
    position isn't between any double quotes it replaces it.
If the line is a directive (check_directive(line) != -1) then if it is:
    -define: gets symbol and value and adds it to hash table;
    -undefine: gets symbol, and if it exists it deletes it;
    -if: transforms argument to int and verifies it's validity then sets values
    for if logic variables
    -else, endif, ifndef, undef analog if


