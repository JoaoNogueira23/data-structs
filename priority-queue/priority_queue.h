typedef struct 
{
    char pacitent[20];
    int key;
} Item;

typedef struct 
{
    Item *v; // vetor de itens alocado dinamicamente
    int n, lenght; // n: quantidade de elementos; lenght: tamanho da fila
} PriorityQueue;
