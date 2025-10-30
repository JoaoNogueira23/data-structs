#ifndef HASHING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE_NAMES 31
#define MAX_AMOUNT_ARCHIEVES 2000
#define MAX_AMOUNT_TAGS 30
#define MAX_SIZE_DESCRIBE 100
#define MAX_LEN_OP 20
#define MAX 1783

// estrutura basica dos dados
typedef struct tag{
    char tag_name[MAX_SIZE_NAMES];
} Tag;

typedef struct{
    char archieve_name[MAX_SIZE_NAMES];
    char describe[MAX_SIZE_NAMES];
    char *tags_list; //pilha
    int amount_tags;
} Archieve;
typedef struct {
    char key[MAX_SIZE_NAMES];
} Key;

typedef struct hash_table_files{
    Archieve *array[MAX_AMOUNT_ARCHIEVES];
} HashTableFiles;

typedef struct hash_table_tags{
    Tag *array[MAX_AMOUNT_TAGS];
} HashTableTags;


int hash_function(char *key){
    int n = 0;
    for (int i =0; i<strlen(key); i++){
        n = (256 * n + key[i]) % MAX;
    }

    return n;
}

int insert_tag_filename(Archieve *file, char *tag){
    strcpy(file->tags_list[file->amount_tags], tag);
    file->amount_tags++; // atualizando o tamanho da pilha
};

// inserir novo arquivo
void process_tags_input(char *tags_string, Archieve *file, int amount_tags){
    int tags_inserted = 0;

    const char *delimiter = " ";

    char *tag = strtok(tags_string, delimiter);
    while(tag != NULL){
        if (strlen(tag)>0){
            if(tags_inserted == amount_tags){
                break;
            }
            if(insert_tag_filename(file, tag) == 0){
                tags_inserted++;
            }else{
                printf("Erro inesperado na insercao de tag");
            }
        }

        tag = strtok(NULL,delimiter);
    }
}

void insert_file(char *filename, char *describe, char *tags_list_str, HashTableFiles *files, int amount_tags){
    

    // build archieve
    Archieve *archieve_inserted = (Archieve *)malloc(sizeof(Archieve));
    strcpy(archieve_inserted->archieve_name, filename);
    strcpy(archieve_inserted->describe, describe);
    archieve_inserted->amount_tags = 0;

    // inserindo as tags
    process_tags_input(tags_list_str, archieve_inserted, amount_tags);

    // inserindo o arquivo na tabela
    int key_to_file = hash_function(filename) % MAX_AMOUNT_ARCHIEVES;

    // tratando colisoes com sondagem linearchat
    while(files->array[key_to_file] != NULL){
        // tratamento linear
        key_to_file = (key_to_file + 1) % MAX_AMOUNT_ARCHIEVES;
    }

    files->array[key_to_file] = archieve_inserted;
    
};

void remove_file(char *filename, HashTableFiles *files){
    int key_file = hash_function(filename) % MAX_AMOUNT_ARCHIEVES;

    // loop para validar se a key é a correta ou existe colisao
    for(int i = 0; i< MAX_AMOUNT_ARCHIEVES;i++){
        Archieve *file = files->array[key_file];
        if(file != NULL && strcmp(file->archieve_name, filename) == 0){
            free(file);
            files->array[key_file] == NULL;
            return;
        }

        // atualizando a key
        int key_file = (key_file + i) % MAX_AMOUNT_ARCHIEVES;
    }

    free(files->array[key_file]);
}

#endif // AGENDA_H