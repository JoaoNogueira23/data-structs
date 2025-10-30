#include "hashtags.h"



int main(){
    char op[MAX_LEN_OP];

    HashTableFiles *files;
    HashTableTags *tags;
    
    while(scanf("%s", op) && strcmp(op, "ENCERRAR") != 0){
        if(strcmp(op, "INSERIR ARQUIVO") == 0){
            char filename[MAX_SIZE_NAMES];
            char describe_file[MAX_SIZE_DESCRIBE];
            int amount_tags_file;
            scanf("%s", filename);
            scanf("%s", describe_file);
            scanf("%d", &amount_tags_file);

            int aux_for_allocation = amount_tags_file*MAX_SIZE_NAMES + amount_tags_file;
            char tags_list_str[aux_for_allocation];
            scanf("%s", tags_list_str);
            insert_file(filename, describe_file, tags_list_str, files, amount_tags_file);            

        }else if(strcmp(op, "REMOVER ARQUIVO") == 0){
            char filename_remove[MAX_SIZE_NAMES];
            scanf("%s", filename_remove);
            remove_file(filename_remove, files);
        }else if(strcmp(op, "ALTERAR ARQUIVO") == 0){
            char filename_changed[MAX_SIZE_NAMES];
            char new_filename[MAX_SIZE_NAMES];
            char new_describe_file[MAX_SIZE_DESCRIBE];
            change_file(filename_changed, new_filename, new_describe_file, files);
            // chama a funcao
        }else if(strcmp(op, "INSERIR TAG") == 0){
            char filename[MAX_SIZE_NAMES];
            char tag_to_insert[MAX_SIZE_NAMES];
        }else if(strcmp(op, "REMOVER TAG") == 0){
            char filename[MAX_SIZE_NAMES];
            char tag_to_remove[MAX_SIZE_NAMES];
        }else if(strcmp(op, "BUSCAR TAG") == 0){
            char tag_to_search[MAX_SIZE_NAMES];
            scanf("%s", tag_to_search);
            printf("Busca por tag: %s", tag_to_search);
        }else if(strcmp(op, "BUSCAR ARQUIVO") == 0){
            char archieve_to_search[MAX_SIZE_NAMES];
            scanf("%s", archieve_to_search);
            printf("Acessando arquivo: %s", archieve_to_search);
        }
    }
    
    return 0;
}