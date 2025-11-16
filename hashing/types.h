#ifndef TYPES_H
#define MAX_TIME 600 // tempo maximo para atividades na agenda
#define MAX_COMMAND_LEN 100

#define MAX_SIZE_NAMES 31
#define MAX_AMOUNT_ARCHIEVES 2000
#define MAX_AMOUNT_TAGS 30
#define MAX_SIZE_DESCRIBE 101
#define MAX_LEN_OP 100
#define MAX 1783
#define DELETED_FLAG ((Archieve*)-1)
#define HT_SIZE_TAGS 97

typedef struct File File;
// Declaração Antecipada para 'Tag' (se for o primeiro a ser usado)
typedef struct Tag Tag;

/* Tag struct */
typedef struct Tag {
    char name[MAX_SIZE_NAMES];       // alocado uma vez
    File **files;     // array dinâmico de File* (lista invertida)
    int file_count;
} Tag;
typedef struct File {
    char name[MAX_SIZE_NAMES];                // alocado uma vez
    char description[MAX_SIZE_DESCRIBE];         // alocado uma vez
    Tag **tags;                // array dinâmico de Tag*
    int total_tags;
    int insertion_index;       // para manter ordem global de insercao
} File;

typedef struct FileSlot {
    File *file; // NULL se EMPTY ou DELETED
} FileSlot;

typedef struct HashTableFiles {
    FileSlot **slots;
    int size;
} HashTableFiles;

typedef struct{
    Tag *data;
} TagSlot;

typedef struct HashTableTags {
    Tag *array[MAX_AMOUNT_TAGS];
    TagSlot **slots;
} HashTableTags;


extern int g_insertion_counter; // lastrear a ordem de insercao de arquivos

#endif