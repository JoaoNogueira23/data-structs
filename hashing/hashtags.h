#ifndef HASHING_H
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hash_function(char *key, int factor){
    int n = 0;
    for (int i =0; i<strlen(key); i++){
        n = (256 * n + key[i]) % factor;
    }

    return n;
}

File *find_file(char *filename, HashTableFiles *files){
    int key_file = hash_function(filename, MAX_AMOUNT_ARCHIEVES);
    // loop para validar se a key é a correta ou existe colisao
    File *file = files->slots[key_file];
    for(int i = 1; i< MAX_AMOUNT_ARCHIEVES;i++){
        if(file != NULL && strcmp(file->name, filename) == 0){
            // arquivo encontrado
            break;
        }

        // atualizando a key
        key_file = (key_file + i) % MAX_AMOUNT_ARCHIEVES;
        file = files->slots[key_file];
    }
}

Tag *find_tag(char *tagname, HashTableTags *tags){
    int key_tag = hash_function(tagname, MAX_AMOUNT_TAGS);

    Tag *tag_to_remove = tags->array[key_tag];
    for(int i = 1; i< MAX_AMOUNT_TAGS;i++){
        if(tag_to_remove != NULL && strcmp(tag_to_remove->name, tagname) == 0){
            // arquivo encontrado
            break;
        }

        // atualizando a key
        key_tag = (key_tag + i) % MAX_AMOUNT_TAGS;
        tag_to_remove = tags->array[key_tag];
    }
}



int add_tag_to_file(File *file, char *tagname, HashTableTags *tags){
    Tag *tag = (Tag *)malloc(sizeof(Tag));
    strcpy(tag->name, tagname);

    // inserindo um ponteiro para o arquivo no array de indice invertido das tags
    tag->files[tag->file_count++] = file;

    // inserindo tag na hash table de tags
    int index_to_tag = hash_function(tagname, MAX_AMOUNT_TAGS);

    // tratando posiveis colisoes
    for(int i = 1; i< MAX_AMOUNT_TAGS;i++){
        if(tag != NULL && strcmp(tag->name, tagname) == 0){
            // arquivo encontrado
            break;
        }

        // atualizando a key
        index_to_tag = (index_to_tag + i) % MAX_AMOUNT_TAGS;
    }

    tags->array[index_to_tag] = tag;

    // inserindo os indices invertidos para cada tag no arquivo (referencia para as tags que o arquivo possui)
    file->tags[file->total_tags++] = tag;
};

// inserir novo arquivo
void insert_tags(char *tags_string, File *file, HashTableTags *tags){
    int tags_inserted = 0;
    int amount_tags = file->total_tags;

    const char *delimiter = " ";

    char *tag = strtok(tags_string, delimiter);
    while(tag != NULL){
        if (strlen(tag)>0){
            if(tags_inserted == amount_tags){
                break;
            }
            if(add_tag_to_file(file, tag, tags) == 0){
                tags_inserted++;
            }else{
                printf("Erro inesperado na insercao de tag");
            }
        }

        tag = strtok(NULL,delimiter);
    }
}


void insert_file_to_hashfile(File *archieve_inserted, HashTableFiles *files){
    // inserindo o arquivo na hash table dos arquivos
    int key_to_file = hash_function(archieve_inserted->name, MAX_AMOUNT_ARCHIEVES);

    // tratando colisoes com sondagem linearchat
    for(int i = 1; i < MAX_AMOUNT_TAGS; i++){
        if (files->slots[key_to_file] != NULL){
            break;
        }

        // sondagem linear
        key_to_file = (key_to_file + i) % MAX_AMOUNT_TAGS;
    }
    files->slots[key_to_file] = archieve_inserted;
};

void remove_file(char *filename, HashTableFiles *files){
    int key_file = hash_function(filename, MAX_AMOUNT_ARCHIEVES); 

    // loop para validar se a key é a correta ou existe colisao
    for(int i = 1; i< MAX_AMOUNT_ARCHIEVES;i++){
        File *file = files->slots[key_file];
        // arquivo resgatado
        if(file != NULL && strcmp(file->name, filename) == 0){
            files->slots[key_file] = NULL;
            return;
        }

        // atualizando a key
        key_file = (key_file + i) % MAX_AMOUNT_ARCHIEVES;
    }

    files->slots[key_file] = NULL;
}


void change_file(char *filename, char *new_filename, char *new_describe, HashTableFiles *files){
    int key_file = hash_function(filename, MAX_AMOUNT_ARCHIEVES);
    // loop para validar se a key é a correta ou existe colisao
    File *file = find_file(filename, files);

    strcpy(file->name, new_filename);
    strcpy(file->description, new_describe);
}

void insert_tag(char *filename, char *new_tag, HashTableFiles *files, HashTableTags *tags){
    int key_file = hash_function(filename, MAX_AMOUNT_ARCHIEVES);
    // loop para validar se a key é a correta ou existe colisao
    File *file_to_insert_tag = find_file(filename, files);
    add_tag_to_file(file_to_insert_tag, new_tag, tags);
}

void remove_tag(char *filename, char *tag_to_remove, HashTableFiles *files){
    File *file = find_file(filename, files);

    // fazer um algoritmo de busca da tag no array dinamico

}


#endif // AGENDA_H