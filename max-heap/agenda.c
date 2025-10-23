#include "agenda.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TIME 600 // tempo maximo para atividades na agenda
#define MAX_COMMAND_LEN 100

void build_agenda(PriorityQueue *max_heap, PriorityQueue *min_heap){
    int current_time = MAX_TIME;

    Item current_atividade;

    printf("--AGENDA--\n");

    // loop para printar as atividades de maior duracao, de acordo com a prioridade
    while(max_heap->n > 0){
        current_atividade = max_heap->v[0];

        if(current_time >= current_atividade.duration){
            current_atividade = max_extrai_maximo(max_heap);
            remove_item_by_id(min_heap, current_atividade.id, 0);

            printf("(%d) %s - Prioridade %d - Estimativa: %d minutos\n",
                current_atividade.id, current_atividade.atividade, 
                current_atividade.priority, current_atividade.duration
            );

            current_time -= current_atividade.duration;
        }else{
            break; // o tempo ja foi consumido
        }
    }

    // analogo, mas agora pegando as atividades para complementar de acordo com a menor duracao
    while(current_time > 0 && min_heap->n >0){
        current_atividade = min_heap->v[0];
        //
        if(current_time >= current_atividade.duration){
            current_atividade = min_extrai_minimo(min_heap);
            remove_item_by_id(max_heap, current_atividade.id, 1);

            printf("(%d) %s - Prioridade %d - Estimativa: %d minutos\n",
                current_atividade.id, current_atividade.atividade, 
                current_atividade.priority, current_atividade.duration
            );

            current_time -= current_atividade.duration;
        }else{
            break;
        }
    }

    printf("----------\n");
}

int main(){
    //
    int max_atividades;
    scanf("%d", &max_atividades);

    PriorityQueue *max_heap = criar_filaprio(max_atividades);
    PriorityQueue *min_heap = criar_filaprio(max_atividades);

    char op[MAX_COMMAND_LEN];

    while(scanf("%s", op) && strcmp(op, "encerrar") != 0){
        if(strcmp(op, "atividade") == 0){
            // logica de inserir atividade
            int id, priority, duration;
            Item new_item;
            // leitura dos parametros
            scanf("%d %d %d",&id, &priority, &duration);
            // leitura do nome da atividade
            char name[151];
            scanf(" %[^\n]", name);

            new_item.id = id;
            new_item.priority = priority;
            new_item.duration = duration;
            strcpy(new_item.atividade, name);

            max_insere(max_heap, new_item);
            min_insere(min_heap, new_item);
            // ajustar o print do tamanho do heap
            printf("%s adicionada - Atividades futuras: %d\n", name, max_heap->n);

        }
        else if(strcmp(op, "agenda") == 0){
            build_agenda(max_heap, min_heap);
        }else if(strcmp(op, "altera") == 0){
            // leitura de resgate de dados
            int id, delta_priority;
            scanf("%d %d", &id, &delta_priority);

            int pos_max = get_pos_by_id(max_heap, id);
            int pos_min = get_pos_by_id(min_heap, id);

            Item *item_max = &max_heap->v[pos_max];
            Item *item_min = &min_heap->v[pos_min];
            // nova prioridade aa
            int old_priority = item_max->priority;
            int new_priority_max = old_priority + delta_priority;

            int old_priority_min = item_min->priority;
            int new_priority_min = old_priority_min +delta_priority;
            //
            printf("Alterado %s %d -> %d\n", item_max->atividade,
                old_priority, new_priority_max
            );

            // reajustando itens nos heaps
            max_change_priority(max_heap, pos_max, new_priority_max);
            min_change_duration(min_heap, pos_min, new_priority_min);

              
        }
    }

    printf("Restaram %d atividades\n", max_heap->n);
    
    free_fprio(max_heap);
    free_fprio(min_heap);
    return 0;
}