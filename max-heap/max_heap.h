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
#define F_LEFT(i) (2*i + 1)
#define F_RIGHT(i) (2*i + 2)

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

// ajusta a regra do pai ser o valor maximo
void sobe_no_heap(PriorityQueue *fprio, int k){
    if(k > 0 && fprio -> v[PAI(k)].key < fprio->v[k].key){
        swap(&fprio->v[k], &fprio->v[PAI(k)]);
        sobe_no_heap(fprio, PAI(k));
    }
}

void insere(PriorityQueue *fprio, Item item) {
    fprio->v[fprio->n] = item;
    fprio->n++;
}



void desce_no_heap(PriorityQueue *fprio, int k){
    int maior_filho;
    if(F_LEFT(k) < fprio -> n){
        maior_filho = F_LEFT(k);
        if(F_RIGHT(k) < fprio -> n && fprio->v[F_LEFT(k)].key < fprio->v[F_RIGHT(k)].key)
            maior_filho = F_RIGHT(k);
        if(fprio->v[k].key < fprio->v[maior_filho].key) {
            swap(&fprio->v[k], &fprio->v[maior_filho]);
            desce_no_heap(fprio, maior_filho);
        }  
    }
}


Item extrai_maximo(PriorityQueue *fprio) {
    /*
    A diferença aqui para a fila de prioridade é que precisa ajustar os 
    filhos para que o pai tenha os filhos esquerdo e direito
    */
    Item item = fprio->v[0];
    swap(&(fprio->v[0]), &(fprio->v[fprio->n-1]));
    fprio->n--;
    desce_no_heap(fprio, 0);
    return item;
}