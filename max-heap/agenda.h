#ifndef AGENDA_H
#define AGENDA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    char atividade[151];
    int duration;
    int priority;
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

int get_pos_by_id(PriorityQueue *fprio, int id){
    return fprio->pos[id];
}

Item *get_item_by_id(PriorityQueue *fprio, int id){
    int pos = get_pos_by_id(fprio, id);
    if(pos != -1){
        return &fprio->v[pos];
    }
    return NULL;
}

int size_fprio(PriorityQueue *fprio){
    if(!fprio) return 0;
    return fprio->n;
}

// funcoes de comparacao para prioridade 
int max_heap_maior(Item item1, Item item2){
    // retorna 1 caso o item1 tenha prioridade
    if (item1.priority != item2.priority) {
        return item1.priority > item2.priority;
    }
    if (item1.duration != item2.duration) {
        return item1.duration > item2.duration;
    }
    return item1.id < item2.id; 
}

int min_heap_maior(Item item1, Item item2) {
    // retorna 1 caso o item1 tenha prioridade
    if (item1.duration != item2.duration) {
        return item1.duration < item2.duration;
    }
    if (item1.priority != item2.priority) {
        return item1.priority > item2.priority;
    }
    return item1.id < item2.id;
}

void swap_itens(PriorityQueue *fprio, int i, int j){
    if(!fprio || i<0 || j<0 || i>= fprio->n || j>= fprio->n) return;

    // atualiza itens no vetor
    Item temp = fprio->v[i];
    fprio->v[i] = fprio->v[j];
    fprio->v[j] = temp;

    // atualiza os IDs no vetor de posicoes
    fprio->pos[fprio->v[i].id] = i;
    fprio->pos[fprio->v[j].id] = j;
}

PriorityQueue *criar_filaprio(int tam) {
    PriorityQueue *fprio = (PriorityQueue*) malloc(sizeof(PriorityQueue));
    fprio->v = (Item*) malloc(tam * sizeof(Item));

    // inicializando o vetor posicoes
    fprio->pos = (int*) malloc((tam+1) * sizeof(int));
    for(int i = 0; i < tam; i++){
        fprio->pos[i] = -1; // valor para indicar que o id nao tem item no heap
    }
    
    fprio->n = 0;
    fprio->lenght = tam;
    fprio->next_id = 1;
    return fprio;
}

void free_fprio(PriorityQueue *fprio){
    if(fprio == NULL) return;

    if (fprio->v != NULL){
        free(fprio->v);
    }
    if(fprio->pos != NULL){
        free(fprio->pos);
    }
    fprio->n = 0;
    fprio->lenght = 0;

    free(fprio);
}

void max_sobe_no_heap(PriorityQueue *fprio, int k){
    if(k > 0 && max_heap_maior(fprio->v[k], fprio->v[PAI(k)])){
        swap_itens(fprio, k, PAI(k));
        max_sobe_no_heap(fprio, PAI(k));
    }
}

void max_desce_no_heap(PriorityQueue *fprio, int k){
    int maior_filho;
    if(F_LEFT(k) < fprio -> n){
        maior_filho = F_LEFT(k);
        
        if(F_RIGHT(k) < fprio -> n && max_heap_maior(fprio->v[F_RIGHT(k)], fprio->v[F_LEFT(k)]))
            maior_filho = F_RIGHT(k);

        if(max_heap_maior(fprio->v[maior_filho], fprio->v[k])) {
            swap_itens(fprio, k, maior_filho);
            max_desce_no_heap(fprio, maior_filho);
        }  
    }
}

void min_sobe_no_heap(PriorityQueue *fprio, int k){
    // Se o filho (k) é MAIOR que o pai (PAI(k))
    if(k > 0 && min_heap_maior(fprio->v[k], fprio->v[PAI(k)])){
        swap_itens(fprio, k, PAI(k));
        min_sobe_no_heap(fprio, PAI(k));
    }
}

void min_desce_no_heap(PriorityQueue *fprio, int k){
    int maior_filho;
    if(F_LEFT(k) < fprio -> n){
        maior_filho = F_LEFT(k);
        
        if(F_RIGHT(k) < fprio -> n && min_heap_maior(fprio->v[F_RIGHT(k)], fprio->v[F_LEFT(k)]))
            maior_filho = F_RIGHT(k);

        if(min_heap_maior(fprio->v[maior_filho], fprio->v[k])) {
            swap_itens(fprio, k, maior_filho);
            min_desce_no_heap(fprio, maior_filho);
        }  
    }
}

void min_insere(PriorityQueue *fprio, Item item) {
    if (!fprio || fprio->n >= fprio->lenght) return;
    
    int k = fprio->n;
    
    fprio->v[k] = item;
    fprio->pos[item.id] = k;
    fprio->n++;

    //organizando heap
    min_sobe_no_heap(fprio, k);
}

void max_insere(PriorityQueue *fprio, Item item) {
    if (!fprio || fprio->n >= fprio->lenght) return;
    
    int k = fprio->n;

    fprio->v[k] = item;
    fprio->pos[item.id] = k;
    fprio->n++;

    //organizando heap
    max_sobe_no_heap(fprio, k);
}

Item min_extrai_minimo(PriorityQueue *fprio) {
    Item item = fprio->v[0]; 
    swap_itens(fprio, 0, fprio->n-1);

    fprio->n--;
    if (fprio->n > 0) min_desce_no_heap(fprio, 0);

    fprio->pos[item.id] = -1; // removeu
    return item;
}

// O Min-Heap também precisa ser reajustado quando a prioridade muda, 


Item max_extrai_maximo(PriorityQueue *fprio) {
    /*
    A diferença aqui para a fila de prioridade é que precisa ajustar os 
    filhos para que o pai tenha os filhos esquerdo e direito
    */
    Item item = fprio->v[0]; // a raiz é sempre o item max no heap
    swap_itens(fprio, 0, fprio->n-1);

    fprio->n--;
    max_desce_no_heap(fprio, 0);

    fprio->pos[item.id] = -1; // removeu
    return item;
}

Item extrai_minimo(PriorityQueue *fprio) {
    Item item = fprio->v[0]; 
    swap_itens(fprio, 0, fprio->n-1);

    fprio->n--;
    if (fprio->n > 0) min_desce_no_heap(fprio, 0);

    fprio->pos[item.id] = -1; // removeu
    return item;
}

// pois a prioridade é um critério de desempate.
void min_change_duration(PriorityQueue *fprio, int k, int new_valeu){
    fprio->v[k].priority = new_valeu;
    
    if (k > 0 && min_heap_maior(fprio->v[k], fprio->v[PAI(k)])){
        min_sobe_no_heap(fprio, k);
    } else {
        min_desce_no_heap(fprio, k);
    }
}


void max_change_priority(PriorityQueue *fprio, int k, int new_value){
    fprio->v[k].priority = new_value;

    if (k > 0 && max_heap_maior(fprio->v[k], fprio->v[PAI(k)])){
        max_sobe_no_heap(fprio, k);
    } else {
        max_desce_no_heap(fprio, k);
    }
}





Item remove_item_by_id(PriorityQueue *fprio, int id, int is_max_heap){
    int k = get_pos_by_id(fprio, id);
    Item item_to_remove = fprio->v[k];

    // troca o item de k com o ultimo item
    swap_itens(fprio, k, fprio->n-1);
    fprio->n--; // removido

    if(fprio->n > k){
        if(is_max_heap){
            if(k>0 && max_heap_maior(fprio->v[k], fprio->v[PAI(k)])){
                max_sobe_no_heap(fprio, k);
            }else{
                max_desce_no_heap(fprio, k);
            }
        }else{
            if(k>0 && min_heap_maior(fprio->v[k], fprio->v[PAI(k)])){
                min_sobe_no_heap(fprio, k);
            }else{
                min_desce_no_heap(fprio, k);
            }
        }
    }
    // liberar item no vetor V e na queue
    fprio->pos[item_to_remove.id] = -1;
    return item_to_remove;
}

#endif // AGENDA_H