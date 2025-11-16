#ifndef HASHING_H
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int hash_function(char *key, int factor){
    int n = 0;
    for (int i =0; i<strlen(key); i++){
        n = (256 * n + key[i]) % factor;
    }

    return n;
}

HashTableFiles *create_hashtable_file(){
    HashTableFiles *ht_files =(HashTableFiles *)malloc(sizeof(HashTableFiles));

    ht_files->slots = (FileSlot **)calloc(MAX_AMOUNT_ARCHIEVES, sizeof(FileSlot *));

    return ht_files;
}

HashTableTags *create_ht_tag(){
    HashTableTags *ht_tags = malloc(sizeof(HashTableTags));
    for (int i = 0; i < MAX_AMOUNT_TAGS; i++)
        ht_tags->array[i] = NULL;
    ht_tags->slots = NULL;
    return ht_tags;
}

File *find_file(char *filename, HashTableFiles *files){
    int key_file = hash_function(filename, MAX_AMOUNT_ARCHIEVES);
    // loop para validar se a key é a correta ou existe colisao
    File *file = NULL;

    for(int i = 0; i< MAX_AMOUNT_ARCHIEVES;i++){
        int current_key = (key_file + i) % MAX_AMOUNT_ARCHIEVES;

        if(files->slots[current_key] == NULL){
            return NULL;
        }

        file = files->slots[current_key]->file;

        if(file == NULL){
            continue;
        }

        if(strcmp(file->name, filename) == 0){
            return file; // Arquivo encontrado
        }
    }

    return file;
}

File *find_file(char *filename, HashTableFiles *files) {
    int key_file = hash_function(filename, MAX_AMOUNT_ARCHIEVES);

    for (int i = 0; i < MAX_AMOUNT_ARCHIEVES; i++) {
        int current_key = (key_file + i) % MAX_AMOUNT_ARCHIEVES;
        FileSlot *slot = files->slots[current_key];

        if (slot == NULL) {
            // encontrou espaço vazio -> chave não está na tabela
            // MAS só podemos garantir isso se não houver "tombstones" (remoções)
            return NULL;
        }

        if (slot->file == NULL) {
            // posição deletada, segue sondando
            continue;
        }

        if (strcmp(slot->file->name, filename) == 0) {
            return slot->file;
        }
    }

    return NULL; // não achou após sondar tudo
}


Tag *find_tag(char *tagname, HashTableTags *tags){
    int key_tag = hash_function(tagname, MAX_AMOUNT_TAGS);

    Tag *tag = tags->array[key_tag];
    for(int i = 1; i< MAX_AMOUNT_TAGS;i++){
        int current_key = (key_tag + i) % MAX_AMOUNT_TAGS;
        Tag *current_tag = tags->array[current_key];

        if (current_tag == NULL){
            // tag nao foi inserida
            return NULL;
        }

        if(strcmp(tag->name, tagname) == 0){
            // tag encontrada
            return current_tag;
        }

    }
    return NULL;
}

int add_tag_to_file(File *file, char *tagname, HashTableTags *tags, int init_total_tag) {
    Tag *current_tag = find_tag(tagname, tags);

    if (init_total_tag == 0) {
        file->tags = (Tag **) calloc(MAX_AMOUNT_TAGS, sizeof(Tag *));
        file->total_tags = 0;
    }

    if (current_tag == NULL) {
        // criar nova tag
        current_tag = (Tag *) malloc(sizeof(Tag));
        strcpy(current_tag->name, tagname);

        current_tag->files = (File **) calloc(MAX_AMOUNT_ARCHIEVES, sizeof(File *));
        current_tag->file_count = 0;

        current_tag->files[current_tag->file_count++] = file; 
    } else {
        current_tag->files[current_tag->file_count++] = file;
    }

    file->tags[init_total_tag] = current_tag;

    if (init_total_tag >= file->total_tags) {
        file->total_tags = init_total_tag + 1;
    }

    return 1;
}

// inserir novo arquivo
void insert_tags_from_new_file(char *tags_string, File *file, HashTableTags *tags){
    int tags_inserted = 0;

    const char *delimiter = " ";

    char *tag = strtok(tags_string, delimiter);
    while(tag != NULL){
        if (strlen(tag)>0){
            if(add_tag_to_file(file, tag, tags, tags_inserted)){
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
    for(int i = 0; i < MAX_AMOUNT_ARCHIEVES; i++){
        int current_key = (key_to_file + i) % MAX_AMOUNT_ARCHIEVES;

        if (files->slots[current_key] == NULL){
            files->slots[current_key] = (FileSlot *)malloc(sizeof(FileSlot));
            if (files->slots[current_key] == NULL) {
                return; 
            }
            
            files->slots[current_key]->file = archieve_inserted;
            
            return;
        }
    }
};

void remove_file(char *filename, HashTableFiles *files){
    int key_file = hash_function(filename, MAX_AMOUNT_ARCHIEVES); 

    // loop para validar se a key é a correta ou existe colisao
    for(int i = 1; i< MAX_AMOUNT_ARCHIEVES;i++){
        File *file = files->slots[key_file]->file;
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
    // loop para validar se a key é a correta ou existe colisao
    File *file = find_file(filename, files);

    strcpy(file->name, new_filename);
    strcpy(file->description, new_describe);
}

void insert_tag(char *filename, char *new_tag, HashTableFiles *files, HashTableTags *tags){
    // loop para validar se a key é a correta ou existe colisao
    File *file_to_insert_tag = find_file(filename, files);
    add_tag_to_file(file_to_insert_tag, new_tag, tags, file_to_insert_tag->total_tags);

    file_to_insert_tag->total_tags++; //atualizando a quantidade de tags
}


void remove_tag(char *filename, char *tag_name_to_remove, HashTableFiles *files){
    File *file_to_update = find_file(filename, files);

    int tag_index = -1; // Variável para guardar o índice da tag
    for (int i = 0; i < file_to_update->total_tags; i++) {
        if (strcmp(file_to_update->tags[i]->name, tag_name_to_remove) == 0) {
            tag_index = i;
            break; // Tag encontrada
        }
    }
    // Desloca todos os elementos após o 'tag_index' uma posição para trás
    for (int i = tag_index; i < file_to_update->total_tags - 1; i++) {
        file_to_update->tags[i] = file_to_update->tags[i + 1];
    }
    file_to_update->total_tags--;

    // Realocação de Memória
    if (file_to_update->total_tags > 0) {
        // Realoca para o novo tamanho (total_tags)
        Tag **new_tags = (Tag **)realloc(file_to_update->tags, 
                                        file_to_update->total_tags * sizeof(Tag *));
        
        file_to_update->tags = new_tags;

    } else {
        free(file_to_update->tags);
        file_to_update->tags = NULL; 
    }
    
}

void search_by_tag(char *tagname, HashTableTags *tags){
    Tag *tag = find_tag(tagname, tags); // sem colisoes faz a busca em O(1)

    printf("Busca por %s\n", tagname);

    for (int i = 0; i<tag->file_count; i++){
        printf("%s\n", tag->files[i]->name);
    }

    printf("----------\n");

}

void search_by_file(char *filename, HashTableFiles *files){
    File *file = find_file(filename, files);

    if (file != NULL){
        printf("Descrição: %s\n", file->description);

        printf("Tags: ");
        for(int i = 0; i < file->total_tags && file->tags[i] != NULL; i++){
            printf("%s ", file->tags[i]->name);
        }
        printf("----------\n");
    }else{
        printf("Arquivo %s não existe\n", filename);
    }

}


#endif // AGENDA_H