#ifndef TYPES_H
#define MAX_TIME 600 // tempo maximo para atividades na agenda
#define MAX_COMMAND_LEN 100

#define MAX_SIZE_NAMES 31
#define MAX_AMOUNT_ARCHIEVES 2001
#define MAX_AMOUNT_TAGS 31
#define MAX_SIZE_DESCRIBE 101
#define MAX_LEN_OP 20
#define MAX 1783
#define DELETED_FLAG ((Archieve*)-1)
#define HT_SIZE_TAGS 97

typedef enum {EMPTY, OCCUPIED, DELETED} SlotStatus;

typedef enum { EMPTY, OCCUPIED, DELETED } SlotStatus;

typedef struct File {
    char name[MAX_SIZE_NAMES];                // alocado uma vez
    char description[MAX_SIZE_DESCRIBE];         // alocado uma vez
    Tag **tags;                // array dinâmico de Tag*
    int total_tags;
    int insertion_index;       // para manter ordem global de insercao
} File;

typedef struct FileSlot {
    SlotStatus status;
    File *file; // NULL se EMPTY ou DELETED
} FileSlot;

typedef struct HashTableFiles {
    FileSlot *slots[MAX_AMOUNT_ARCHIEVES];
    int size;
} HashTableFiles;

/* Tag struct */
typedef struct Tag {
    char name[MAX_SIZE_NAMES];       // alocado uma vez
    File **files;     // array dinâmico de File* (lista invertida)
    int file_count;
} Tag;

typedef struct HashTableTags {
    Tag *array[MAX_AMOUNT_TAGS];
} HashTableTags;


extern int g_insertion_counter; // lastrear a ordem de insercao de arquivos

#endif