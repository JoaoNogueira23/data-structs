/// Max Heap ///
/*
Estrutura interessante para quando o interesse é resgatar o valor
máximo
*/

/*
1. filhos menores ou iguais ao pai (ou seja, a raiz é o máximo)
obs: nao tem uma definição se o filho da esquerda ou direita é maior que o outro

*/

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

#define PAI(i) ((i-1)/2)

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}
PriorityQueue criar_filaprio(int tam) {
    PriorityQueue *fprio = malloc(sizeof(PriorityQueue));
    fprio->v = malloc(tam * sizeof(Item));
    fprio->n = 0;
    fprio->lenght = tam;
    return *fprio;
}

void insere(PriorityQueue *fprio, Item item) {
    fprio->v[fprio->n] = item;
    fprio->n++;
}


Item extrai_maximo(PriorityQueue *fprio) {
    int j, max = 0;
    for (j = 1; j < fprio->n; j++){
        if (fprio->v[max].key < fprio->v[j].key)
        max = j;
    }
    swap(&(fprio->v[max]), &(fprio->v[fprio->n-1]));
    fprio->n--;
    return fprio->v[fprio->n];
}