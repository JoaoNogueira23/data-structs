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
    char name[20];
    int key;
    int id;
} Item;

typedef struct 
{
    Item *v; // vetor de itens alocado dinamicamente
    int n, lenght; // n: quantidade de elementos; lenght: tamanho da fila
    int *pos; // veitor que armazena as posicoes (IDs)
    int next_id;
} PriorityQueue;

#define PAI(i) ((i-1)/2)
#define F_LEFT(i) (2*i + 1)
#define F_RIGHT(i) (2*i + 2)

void swap_itens(PriorityQueue *fprio, int i, int j){
    // atualiza itens no vetor
    Item temp = fprio->v[i];
    fprio->v[i] = fprio->v[j];
    fprio->v[j] = temp;

    // atualiza os IDs no vetor de posicoes
    fprio->pos[fprio->v[i].id] = i;
    fprio->pos[fprio->v[j].id] = j;
}

PriorityQueue criar_filaprio(int tam) {
    PriorityQueue *fprio = malloc(sizeof(PriorityQueue));
    fprio->v = malloc(tam * sizeof(Item));

    // inicializando o vetor posicoes
    fprio->pos = malloc(tam * sizeof(int));
    for(int i = 0; i < tam; i++){
        fprio->pos[i] = -1; // valor para indicar que o id nao tem item no heap
    }
    
    fprio->n = 0;
    fprio->lenght = tam;
    return *fprio;
}

// ajusta a regra do pai ser o valor maximo
void sobe_no_heap(PriorityQueue *fprio, int k){
    if(k > 0 && fprio -> v[PAI(k)].key < fprio->v[k].key){
        swap(fprio, k, PAI(k));
        sobe_no_heap(fprio, PAI(k));
    }
}

void insere(PriorityQueue *fprio, Item item) {
    // ajuste de id
    item.id = fprio->next_id++;
    int k = fprio->n;
    
    // inserindo o item no heap
    fprio->v[k] = item;
    fprio->pos[item.id] = k;
    fprio->n++;

    //organizando heap
    sobe_no_heap(fprio, k);
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
    Item item = fprio->v[0]; // a raiz é sempre o item max no heap
    swap(fprio, 0, fprio->n-1);

    fprio->n--;
    desce_no_heap(fprio, 0);

    fprio->pos[item.id] = -1; // removeu
    return item;
}

int get_pos_by_id(PriorityQueue *fprio, int id){
    return fprio->pos[id];
}

void change_priority(PriorityQueue *fprio, int id, int new_value){
    int k = get_pos_by_id(fprio, id);

    if (k == -1 || k >= fprio->n){
        printf("remocao nao permitida, nao ha item na posicao");
        return;
    }

    change_priority(fprio, k, new_value);
}