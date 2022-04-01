#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "array_list.h"
#include "hash_table.h"

#define HASHTABLE_SIZE 1000

//Returns position of hashtag in given line or -1 if there isn't one 
int check_directive(char* line){
    int i, hashtag_pos, line_len;
    hashtag_pos = -1;
    line_len = strlen(line);

    for(i = 0; i < line_len; i++){
        if(line[i] != ' '){
            if(line[i] == '#'){
                hashtag_pos = i;
            }
            break;
        }
    }

    return hashtag_pos;
}

//Returns string without indentation
char *return_clear_line(char *line){
    int i, line_len;
    char *line_start = line;
    line_len = strlen(line);

    for(i = 0; i < line_len; i++){
        if(line[i] == ' '){
            line_start++;
        }
        else
            break;
    }
    return line_start;
}


int main(int argc, char** argv){
    int index, i, arg_len, hashtag_pos, arg_index, no_inp_files, *comm_pos, j,
        no_directories, ret_val, sym_len, map_len, no_occurances, first_pos, k,
        token_count, no_out_files, if_cond, elif_cond;
    char *specif_file, *d_symbol, *d_mapping, *ch_at, *new_symbol, *aux, *faux, 
        *new_mapping, *in_file, *initial_line, *line_copy, *token, *aux2,
        *directive_type, *out_file, *final_line, **possible_directories,
        *key_value, *line_copy2, *word_loc, **token_dict, *possible_path;

         
    int cmd_needs_arg, is_comm, found_in_file, do_write, ifdef_polarity, in_ifdef,
     in_if, inside_else_if, in_elif, curr_cmd_type, ifdef_cond;
    FILE * in_fp, *out_fp;
    size_t line_len, n;
    hashtable* htable;

    line_len = 256;
    arg_index = 0;
    no_inp_files = 0;
    no_out_files = 0;
    no_directories = 0;
    token_count = 0;
    if_cond = 0;
    elif_cond = 0;
    cmd_needs_arg = 0;
    in_ifdef = 0;
    inside_else_if = 0;
    found_in_file = 0;
    in_if = 0;
    in_elif = 0;
    do_write = 1;
    ifdef_cond = 0;
    ifdef_polarity = 1;

    /*
        Current comand type:
        0 - inputfile
        1 - D
        2 - I 
        3 - o 
    */

    curr_cmd_type = 0;

    if(new_hashtable(&htable, HASHTABLE_SIZE) == 12)
        return 12;
    initial_line = calloc(291, sizeof(char));
    if(initial_line == NULL)
        return 12;
    specif_file = calloc(100, sizeof(char));
    if(specif_file == NULL)
        return 12;
    d_symbol = calloc(100, sizeof(char));
    if(d_symbol == NULL)
        return 12;
    d_mapping = calloc(100, sizeof(char));
    if(d_mapping == NULL)
        return 12;
    in_file = calloc(100, sizeof(char));
    if(in_file == NULL)
        return 12;
    out_file = calloc(100, sizeof(char));
    if(out_file == NULL)
        return 12;
    line_copy = calloc(300, sizeof(char));
    if(line_copy == NULL)
        return 12;
    possible_path = calloc(300, sizeof(char));
    if(possible_path == NULL)
        return 12;
    line_copy2 = calloc(300, sizeof(char));
    if(line_copy2 == NULL)
        return 12;
    aux2 = line_copy2;
    final_line = calloc(300, sizeof(char));
    if(final_line == NULL)
        return 12;
    faux = final_line;
    comm_pos = calloc(300, sizeof(int));
    if(comm_pos == NULL)
        return 12;
    possible_directories = (char**)calloc(100, sizeof(char*));
    if(possible_directories == NULL)
        return 12;
    for(i = 0; i < 100; i++){
        possible_directories[i] = (char*) calloc(100, sizeof(char));
        if(possible_directories[i] == NULL)
            return 12;
    }
    token_dict = (char**)calloc(300, sizeof(char*));
    if(token_dict == NULL)
        return 12;
    for(i = 0; i < 300; i++){
        token_dict[i] = (char*) calloc(300, sizeof(char));
        if(token_dict[i] == NULL)
            return 12;
    }

    strcpy(in_file, "stdin");
    strcpy(out_file, "stdout");

    //Argument processing
    for(i = 1; i < argc; i++){
        strcpy(specif_file, argv[i]);
        if(!cmd_needs_arg){
            curr_cmd_type = 0;
        }
        //Verify if current argument is a command
        if(argv[i][0] == '-'){
            arg_len = strlen(argv[i]);

            //-D define symbol 
            if(argv[i][1] == 'D'){
                if(arg_len == 2){
                    cmd_needs_arg = 1;
                    arg_index = i + 1;
                }
                else{
                    cmd_needs_arg = 0;
                    arg_index = i;

                    //Get symbol and mapping from command
                    strncpy(specif_file, argv[i]+2, arg_len);
                    specif_file[arg_len] = '\0';
                    ch_at = strchr(specif_file, '=');
                    if(ch_at != NULL){
                        index = (int)(ch_at - specif_file);
                        strncpy(d_symbol, specif_file, index);
                        strncpy(d_mapping, specif_file+index+1, arg_len); 
                    }
                    else{
                        strcpy(d_symbol, specif_file);
                        strcpy(d_mapping, "");
                    }
                }
                curr_cmd_type = 1;
            }
            //-I include directories
            else if(argv[i][1] == 'I'){
                if(arg_len == 2){
                    cmd_needs_arg = 1;
                    arg_index = i + 1;
                }
                else{
                    cmd_needs_arg = 0;
                    arg_index = i;
                    strncpy(specif_file, argv[i]+2, arg_len);
                    specif_file[arg_len] = '\0';
                }
                curr_cmd_type = 2;
            }
            //-o specify output file
            else if(argv[i][1] == 'o'){
                if(arg_len == 2){
                    cmd_needs_arg = 1;
                    arg_index = i + 1;
                }
                else{
                    arg_index = i;
                    strncpy(specif_file, argv[i]+2, arg_len);
                    specif_file[arg_len] = '\0';
                    cmd_needs_arg = 0;
                    no_out_files++;
                }
                curr_cmd_type = 3;
            }
        }
        if((curr_cmd_type == 1) && (arg_index == i)){
            //Get symbol and mapping from command
            if(cmd_needs_arg){
                cmd_needs_arg = 0;
                
                strcpy(specif_file, argv[i]);

                ch_at = strchr(specif_file, '=');
                if(ch_at != NULL){
                    index = (int)(ch_at - specif_file);
                    strncpy(d_symbol, specif_file, index);
                    strncpy(d_mapping, specif_file+index+1, arg_len+2);
                     
                }
                else{
                    strcpy(d_symbol, specif_file);
                    strcpy(d_mapping, "");
                }
            }

            add_elem_hashtable(htable, d_symbol, d_mapping);
        }
        if((curr_cmd_type == 2)  && (arg_index == i)){
            if(cmd_needs_arg){
                cmd_needs_arg = 0;
                strcpy(specif_file, argv[i]);
            }
            strcpy(possible_directories[no_directories], specif_file);
            no_directories++;
        }
        if((curr_cmd_type == 3) && (arg_index == i)){            
            if(cmd_needs_arg){
                cmd_needs_arg = 0;
                strcpy(specif_file, argv[i]);
                no_out_files++;
            }
            //Error multiple output files specified
            if(no_out_files > 1)
                return -1;
            strcpy(out_file, specif_file);
        }
        if(curr_cmd_type == 0){
            no_inp_files++;

            //Error multiple input files specified
            if(no_inp_files > 1){
                return -1;
            }
            strcpy(in_file, specif_file);
        }
        
    }

    //Set reading method
    if(strncmp(in_file, "stdin", 5) != 0){
        in_fp = fopen(in_file, "r");
        if(in_fp == NULL){
            for(i = 0; i < no_directories; i++){
                memset(possible_path, 0, 300);
                strcat(possible_path, possible_directories[i]);
                strcat(possible_path, "\\");
                strcat(possible_path, in_file);
                
                in_fp = fopen(possible_path, "r");
                if(in_fp != NULL){
                    found_in_file = 1;
                    break;
                }
            }   
        }
        else
            found_in_file = 1;

        //File not found
        if(found_in_file = 0)
            return -1;
    }
    else
        in_fp = stdin;

    //Set writing method
    if(strcmp(out_file, "stdout") != 0){
        out_fp = fopen(out_file, "ab+");
    }
    else
        out_fp = stdout;

    //Reads every line from file
    while((fgets(initial_line, line_len, in_fp)) != NULL){
        strcpy(line_copy, initial_line);
        strcpy(line_copy2, initial_line);
        strcpy(final_line, initial_line);

        //If directive write logic
        if((in_if == 1) || (in_elif == 1)){
            if(if_cond){
                if(inside_else_if == 0){
                    do_write = 1;
                }
                else{
                    do_write = 0;
                }
            }
            else{
                if(elif_cond){
                    if(inside_else_if == 0){
                        do_write = 1;
                    }
                    else{
                        do_write = 0;
                    }
                }
                else{
                    if(inside_else_if == 1){
                        do_write = 1;
                    }
                    else{
                        do_write = 0;
                    }
                }
            }
        }
        else{
            do_write = 1;
        }

        //Ifdef directive write logic
        if(in_ifdef == 1){
            if(ifdef_cond == 1){
                if(inside_else_if == 0){
                    if(ifdef_polarity == 1)
                        do_write = 1;
                    else
                        do_write = 0;
                }
                else{
                    if(ifdef_polarity == 0)
                        do_write = 1;
                    else
                        do_write = 0;
                }
            }
            else{
                if(inside_else_if == 1){
                    if(ifdef_polarity == 1)
                        do_write = 1;
                    else
                        do_write = 0;
                }
                else{
                    if(ifdef_polarity == 0)
                        do_write = 1;
                    else
                        do_write = 0;
                }
            }
        }
        else{
            if((in_if == 0) && (in_elif == 0))
                do_write = 1;
        }

        //Directive undef
        hashtag_pos = check_directive(line_copy);
        if(hashtag_pos != -1){
            directive_type = strstr(line_copy, "undef");
            index = directive_type - line_copy;
            if(directive_type && (index == 1)){
                token = strtok(line_copy, "\t []{}<>=+-*/%!&|^.,:;()\\");

                //Get symbol
                token = strtok(NULL, "\t \n[]{}<>=+-*/%!&|^.,:;()\\");
                strcpy(d_symbol, token);               
                aux = find_key_value_hashtable(htable, d_symbol, 1);
            }
        }

        strcpy(line_copy, final_line);

        //Verifies if any of the words in the line are in the hashtable and
        //replaces them
        for(k = 0; k < token_count; k++){
            memset(token_dict[k], 0, 100);
        }
        token_count = 0;
        token = strtok(line_copy, "\t []{}<>=+-*/%!&|^.,:;()\\");
        while(token != NULL){
            if(token_dict[token_count][0] != '\n'){
                strcpy(token_dict[token_count], token);
                token_count++;
            }
            token = strtok(NULL, "\t \n[]{}<>\"=+-*/%!&|^.,:;()\\");
        }

        //Iterates through tokens
        for(k = 0; k < token_count; k++){
            strcpy(line_copy2, final_line);

            if(key_exists_hashtable(htable, token_dict[k]) == 1){
                key_value = find_key_value_hashtable(htable, token_dict[k], 0);
                for(i = 0; i < 300; i++)
                    comm_pos[i] = 0;

                //Found symbol which is to be replaced
                if(strcmp(key_value, token_dict[k]) != 0){
                    sym_len = strlen(token_dict[k]);
                    map_len = strlen(key_value);

                    //Gets each comment occurance
                    first_pos = -1;
                    for(i = 0; i < strlen(line_copy2); i++){
                        if((int)line_copy2[i] == 34){
                            if(first_pos == -1){
                                first_pos = i;
                            }
                            else{
                                comm_pos[first_pos] = i;
                                first_pos = -1;
                            }
                        }
                    }

                    //Finds and replaces all occurances of symbol with mapping          
                    //from the hashtable    
                    i = 0;
                    while(*line_copy2){
                        word_loc = strstr(line_copy2, token_dict[k]);

                        //Verify if word is commented
                        is_comm = 0;
                        for(j = 0; j < 300; j++){
                            if((comm_pos[j] != 0) && (j < i) && 
                                (comm_pos[j] > i)){
                                is_comm = 1;
                            }
                        }

                        //Replace word
                        if((word_loc == line_copy2) && (is_comm == 0)){
                            strcpy(&final_line[i], key_value);
                            i += map_len;
                            line_copy2 += sym_len;
                        }
                        else{
                            final_line[i++] = *line_copy2++;
                        }
                    }
                    final_line[i] = '\0';
                }
            }
        }

        strcpy(line_copy, final_line);

        //Directives
        if(hashtag_pos != -1){
            do_write = 0;

            //Directive define
            directive_type = strstr(line_copy, "define");
            index = directive_type - line_copy;
            if(directive_type && (index == 1)){
                token = strtok(line_copy, "\t []{}<>=+-*/%!&|^.,:;()\\");

                //Get symbol
                token = strtok(NULL, "\t []{}<>=+-*/%!&|^.,:;()\\");
                strcpy(d_symbol, token);

                //Get Value
                token = strtok(NULL, "\n");
                if((int)token[0] != 10){
                    strcpy(d_mapping, token);
                    d_mapping[strlen(token)] = '\0';
                }
                add_elem_hashtable(htable, d_symbol, d_mapping);
            }
            
            //Directive if
            directive_type = strstr(line_copy, "if");
            index = directive_type - line_copy;
            if(directive_type && (index == 1) && (line_copy[3] == ' ')){
                in_if = 1;
                token = strtok(line_copy, "\t []{}<>=+-*/%!&|^.,:;()\\");

                //Get symbol
                token = strtok(NULL, "\t []{}<>=+-*/%!&|^.,:;()\\");
                strcpy(d_symbol, token);
                if_cond = atoi(d_symbol);
            }

            //Directive else
            directive_type = strstr(line_copy, "else");
            index = directive_type - line_copy;
            if(directive_type && (index == 1)){
                inside_else_if = 1;
            }          

            //Directive elif
            directive_type = strstr(line_copy, "elif");
            index = directive_type - line_copy;
            if(directive_type && (index == 1)){
                in_elif = 1;
                token = strtok(line_copy, "\t []{}<>=+-*/%!&|^.,:;()\\");

                //Get symbol
                token = strtok(NULL, "\t []{}<>=+-*/%!&|^.,:;()\\");
                strcpy(d_symbol, token);
                elif_cond = atoi(d_symbol);
            }

            //Directive ifdef
            directive_type = strstr(line_copy, "ifdef");
            index = directive_type - line_copy;
            if(directive_type && (index == 1)){
                in_ifdef = 1;
                ifdef_polarity = 1;

                //Get symbol
                token = strtok(line_copy, " ");
                token = strtok(NULL, "\n");
                if(token != NULL){
                    ifdef_cond = key_exists_hashtable(htable, token);
                }
                else
                    ifdef_cond = 1;
            }

            //Directive ifndef
            directive_type = strstr(line_copy, "ifndef");
            index = directive_type - line_copy;
            if(directive_type && (index == 1)){
                in_ifdef = 1;
                ifdef_polarity = 0;
                token = strtok(line_copy, "\t []{}<>=+-*/%!&|^.,:;()\\");

                //Get symbol
                token = strtok(NULL, "\t \n[]{}<>=+-*/%!&|^.,:;()\\");
                strcpy(d_symbol, token);
                ifdef_cond = key_exists_hashtable(htable, d_symbol);
            }

            //Directive endif
            directive_type = strstr(line_copy, "endif");
            index = directive_type - line_copy;
            if(directive_type && (index == 1)){
                in_if = 0;
                inside_else_if = 0;
                if_cond = 0;
                elif_cond = 0;
                in_elif = 0;
                in_ifdef = 0;
                ifdef_polarity = 1;
            }

            /*
            //Directive include
            directive_type = strstr(line_copy, "include");
            index = directive_type - line_copy;
            if(directive_type && (index == 1)){
                return 12;
            }
            */
        }
        
        if(do_write){
            fputs(final_line, out_fp);
        }
    }

    free(initial_line);
    free(specif_file);
    free(d_symbol);
    free(d_mapping);
    if(strncmp(in_file, "stdin", 5) != 0)
        fclose(in_fp);
    free(in_file);
    if(strncmp(out_file, "stdout", 6) != 0)
        fclose(out_fp);
    free(out_file);
    free(line_copy);
    free(aux2);
    free(possible_path);
    free(final_line);
    free(comm_pos);
    for ( i = 0; i < 100; i++ )
        free(possible_directories[i]);
    free(possible_directories);
    for ( i = 0; i < 300; i++ )
        free(token_dict[i]);
    free(token_dict);
    free_hashtable(htable);

    return 0;
}