#include "hashtags.h"

void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}


int main(){
    char op[MAX_LEN_OP];
    int g_insertion_counter = 0;

    HashTableFiles *files = create_hashtable_file();
    HashTableTags *tags = create_ht_tag();
    
    
    while(scanf(" %[^\n]", op) && strcmp(op, "ENCERRAR") != 0){
        
        if(strcmp(op, "INSERIR ARQUIVO") == 0){
            File *new_file = (File *)malloc(sizeof(File));
            new_file->insertion_index = g_insertion_counter;
            char temp_line_tags[512];
            // build do arquivoaaaa
            scanf("%s", new_file->name);

            scanf(" %[^\n]", new_file->description);

            scanf(" %[^\n]", temp_line_tags);

        
            // separar tags do numeroaa
            char *tags_list_str = (char *)malloc(1024 * sizeof(char));
            tags_list_str[0] = '\0';
            const char *delimiter = " ";

            char *tag = strtok(temp_line_tags, delimiter);

            while(tag != NULL){
                if (strlen(tag) > 0){
                    if(isdigit(tag[0])){
                        new_file->total_tags = atoi(tag);
                    }
                    else if(!isdigit(tag[0])){
                        strcat(tags_list_str, tag);
                        strcat(tags_list_str, " ");
                    }
                }else{
                    break;
                }

                tag = strtok(NULL, delimiter);
            }

            insert_tags_from_new_file(tags_list_str, new_file, tags);
            insert_file_to_hashfile(new_file, files);
            g_insertion_counter++;

        }
    
        char command_prefix[30]; 
        char filename[MAX_SIZE_NAMES];
        char tagname[MAX_SIZE_NAMES];
        // build
        

        if (sscanf(op, "%s %s", command_prefix, filename) == 2 && strcmp(command_prefix, "REMOVER") == 0 && strcmp(filename, "ARQUIVO") != 0) {
            if (sscanf(op, "REMOVER ARQUIVO %s", filename) == 1) {
                remove_file(filename, files);
            }

        } else if (sscanf(op, "INSERIR TAG %s %s", filename, tagname) == 2) {
            insert_tag(filename, tagname, files, tags);

        
        } else if (sscanf(op, "REMOVER TAG %s %s", filename, tagname) == 2) {
            remove_tag(filename, tagname, files);
        
        
        } else if (sscanf(op, "BUSCAR ARQUIVO %s", filename) == 1) {
            printf("Acessando arquivo: %s\n", filename);
            search_by_file(filename, files);/////

        
        } else if (sscanf(op, "BUSCAR TAG %s", tagname) == 1) {
            search_by_tag(tagname, tags);

        
        } else if(strcmp(op, "ALTERAR ARQUIVO") == 0){
            char filename_changed[MAX_SIZE_NAMES];
            char new_filename[MAX_SIZE_NAMES];
            char new_describe_file[MAX_SIZE_DESCRIBE];

            
            scanf("%s", filename_changed); 
            scanf("%s", new_filename); 
            
            int c; while ((c = getchar()) != '\n' && c != EOF); 
            
            fgets(new_describe_file, MAX_SIZE_DESCRIBE, stdin);
            
            size_t len = strlen(new_describe_file);
            if (len > 0 && new_describe_file[len-1] == '\n') {
                new_describe_file[len-1] = '\0';
            }
            
            change_file(filename_changed, new_filename, new_describe_file, files);
        }
    }
    
    return 0;
}