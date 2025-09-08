#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

// =========================================================================
//                             1. ENUMS E ESTRUTURAS
// =========================================================================

// Define os estados que um nó pode retornar
typedef enum {
    SUCCESS,
    FAILURE,
    RUNNING,
    INVALID 
} NodeStatus;

// Define o tipo de um nó de composição
typedef enum {
    SEQUENCE,
    SELECTOR,
    PARALLEL
} NodeType;

// A estrutura base para todos os nós da árvore
typedef struct Node {
    NodeType type;
    NodeStatus (*tick_function)(struct Node*); // Ponteiro para a função de tick
    const char* name;
    struct Node** children;
    int num_children;
    int current_child_index;
    int success_threshold; // Para nós Parallel
    NodeStatus status; // <-- ESTA LINHA FOI ADICIONADA!
} Node;


// =========================================================================
//                     2. DECLARAÇÕES DE FUNÇÕES
// =========================================================================

NodeStatus tick_sequence(Node* node);
NodeStatus tick_selector(Node* node);
NodeStatus tick_parallel(Node* node);
NodeStatus tick_decorator_repeat(Node* node);

NodeStatus Bateria_Fraca_Node(Node* node);
NodeStatus Contagem_Tempo_Node(Node* node);
NodeStatus Dist_Parede_menor_q_x_Node(Node* node);
NodeStatus Recue_Node(Node* node);
NodeStatus Expressao_Medo_Node(Node* node);
NodeStatus Vire_Node(Node* node);
NodeStatus Dist_chao_maior_q_x_Node(Node* node);
NodeStatus Esta_Sendo_Balancado_Node(Node* node);
NodeStatus Expressao_enjoo_Node(Node* node);
NodeStatus Expressao_raiva_Node(Node* node);
NodeStatus Botao_apertado_Node(Node* node);
NodeStatus Expressao_risada_Node(Node* node);
NodeStatus Direcional_dif_zero_Node(Node* node);
NodeStatus Expressao_adaptativa_Node(Node* node);
NodeStatus Contagem_sono_maior_q_x_Node(Node* node);
NodeStatus Expressao_dormindo_Node(Node* node);
NodeStatus Contagem_tedio_maior_q_x_Node(Node* node);
NodeStatus Expressao_sono_Node(Node* node);
NodeStatus Esta_x_tempo_sem_acao_externa_Node(Node* node);
NodeStatus Expressao_tedio_Node(Node* node);
NodeStatus Valor_aleatorio_Node(Node* node);
NodeStatus Valor_entre_x_e_x_Node(Node* node);
NodeStatus mexer_braco_para_cima_ebaixo_Node(Node* node);
NodeStatus Expressao_sapeca_Node(Node* node);
NodeStatus Valor_foi_y_Node(Node* node);
NodeStatus Mecher_braco_para_cima_Node(Node* node);
NodeStatus Expressao_alegre_Node(Node* node);
NodeStatus Expressao_Padrao_Node(Node* node);
NodeStatus Foi_empurrado_Node(Node* node);
NodeStatus Mostragem_Bateria_Node(Node* node);

Node* criar_recuada();
Node* fallback_humano();
Node* fallback_robo();
Node* fallback_roboaleatorio();
Node* loop();
Node* build_behavior_tree();


// =========================================================================
//                 3. IMPLEMENTAÇÃO DAS FUNÇÕES DE TICK
// =========================================================================

NodeStatus tick_sequence(Node* node) {
    printf("Executando: %s (Sequence)\n", node->name);

    if (node->status == INVALID) {
        node->current_child_index = 0;
    }

    for (; node->current_child_index < node->num_children; ++node->current_child_index) {
        Node* child = node->children[node->current_child_index];
        NodeStatus child_status = child->tick_function(child);

        printf(" -> Status do filho %s: %s\n", child->name,
               (child_status == SUCCESS) ? "SUCCESS" :
               (child_status == FAILURE) ? "FAILURE" : "RUNNING");

        if (child_status == FAILURE) {
            node->status = FAILURE;
            node->current_child_index = 0; // reseta para próxima vez
            return FAILURE;
        }
        if (child_status == RUNNING) {
            node->status = RUNNING;
            return RUNNING;  // 🔴 não continua, trava aqui até resolver
        }
    }

    node->status = SUCCESS;
    node->current_child_index = 0; // reset para próxima execução
    return SUCCESS;
}

NodeStatus tick_selector(Node* node) {
    printf("Executando: %s (Selector)\n", node->name);

    if (node->status == INVALID) {
        node->current_child_index = 0;
    }

    for (; node->current_child_index < node->num_children; ++node->current_child_index) {
        Node* child = node->children[node->current_child_index];
        NodeStatus child_status = child->tick_function(child);

        printf(" -> Status do filho %s: %s\n", child->name,
               (child_status == SUCCESS) ? "SUCCESS" :
               (child_status == FAILURE) ? "FAILURE" : "RUNNING");

        if (child_status == SUCCESS) {
            node->status = SUCCESS;
            node->current_child_index = 0;
            return SUCCESS;
        }
        if (child_status == RUNNING) {
            node->status = RUNNING;
            return RUNNING; // 🔴 trava aqui até sair do RUNNING
        }
    }

    node->status = FAILURE;
    node->current_child_index = 0;
    return FAILURE;
}


NodeStatus tick_parallel(Node* node) {
    printf("Executando: %s (Parallel)\n", node->name);

    int success_count = 0;
    int running_count = 0;

    for (int i = 0; i < node->num_children; ++i) {
        Node* child = node->children[i];
        NodeStatus child_status = child->tick_function(child);

        printf(" -> Status do filho %s: %s\n", child->name,
               (child_status == SUCCESS) ? "SUCCESS" :
               (child_status == FAILURE) ? "FAILURE" : "RUNNING");

        if (child_status == SUCCESS) success_count++;
        if (child_status == RUNNING) running_count++;
    }

    if (success_count >= node->success_threshold) {
        node->status = SUCCESS;
        return SUCCESS;
    }
    if (running_count > 0) {
        node->status = RUNNING;
        return RUNNING;
    }

    node->status = FAILURE;
    return FAILURE;
}


NodeStatus tick_decorator_repeat(Node* node) {
    printf("Executando: %s (Decorator Repeat até Bateria Fraca)\n", node->name);

    // primeiro checa se a bateria está fraca
    NodeStatus bateria_status = node->children[1]->tick_function(node->children[1]);
    if (bateria_status == SUCCESS) {
        printf(" -> Bateria fraca detectada, encerrando repeat.\n");
        return SUCCESS;
    }

    // se não está fraca, continua repetindo o loop normal
    NodeStatus child_status = node->children[0]->tick_function(node->children[0]);
    if (child_status == FAILURE) {
        return FAILURE;
    }

    return RUNNING;
}



// =========================================================================
//                 4. IMPLEMENTAÇÃO DOS NÓS DE COMPORTAMENTO
// =========================================================================

NodeStatus Bateria_Fraca_Node(Node* node) {
    static int execucoes = 0; // persiste entre chamadas
    execucoes++;

    printf("Executando: %s (vez %d)\n", node->name, execucoes);

    if (execucoes >= 60) {
        return SUCCESS; // só retorna sucesso a partir da segunda vez
    } else {
        return FAILURE; // primeira vez retorna failure
    }
}
NodeStatus Contagem_Tempo_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return RUNNING;
}

NodeStatus Dist_Parede_menor_q_x_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return FAILURE;
}

NodeStatus Recue_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Expressao_Medo_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Vire_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Dist_chao_maior_q_x_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return FAILURE;
}

NodeStatus Esta_Sendo_Balancado_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return FAILURE;
}

NodeStatus Expressao_enjoo_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Expressao_raiva_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Botao_apertado_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return FAILURE;
}

NodeStatus Expressao_risada_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Direcional_dif_zero_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Expressao_adaptativa_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Contagem_sono_maior_q_x_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return FAILURE;
}

NodeStatus Expressao_dormindo_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Contagem_tedio_maior_q_x_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return FAILURE;
}

NodeStatus Expressao_sono_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Esta_x_tempo_sem_acao_externa_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return FAILURE;
}

NodeStatus Expressao_tedio_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Valor_aleatorio_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Valor_entre_x_e_x_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return FAILURE;
}

NodeStatus mexer_braco_para_cima_ebaixo_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Expressao_sapeca_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Valor_foi_y_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return FAILURE;
}

NodeStatus Mecher_braco_para_cima_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Expressao_alegre_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Expressao_Padrao_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Foi_empurrado_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return FAILURE;
}

NodeStatus Mostragem_Bateria_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}


// =========================================================================
//                 5. IMPLEMENTAÇÃO DOS CONSTRUTORES DA ÁRVORE
// =========================================================================

Node* criar_node(NodeType type, const char* name, NodeStatus (*tick_function)(Node*), int num_children, Node** children_array, int success_threshold) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        printf("Erro: falha na alocação de memória para o nó %s\n", name);
        return NULL;
    }
    node->type = type;
    node->name = name;
    node->tick_function = tick_function;
    node->children = children_array;
    node->num_children = num_children;
    node->status = INVALID;
    node->current_child_index = 0;
    node->success_threshold = success_threshold;
    return node;
}

Node* criar_behavior_node(const char* name, NodeStatus (*tick_function)(Node*)) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        printf("Erro: falha na alocação de memória para o nó %s\n", name);
        return NULL;
    }
    node->name = name;
    node->tick_function = tick_function;
    node->children = NULL;
    node->num_children = 0;
    node->status = INVALID;
    return node;
}


Node* criar_recuada() {
    Node** children = (Node**)malloc(sizeof(Node*) * 1);
    children[0] = criar_behavior_node("Expressao_Medo", Expressao_Medo_Node);
    return criar_node(SEQUENCE, "Recuada", tick_sequence, 1, children, 0);
}

Node* fallback_humano() {
    Node** botao_children = (Node**)malloc(sizeof(Node*) * 2);
    botao_children[0] = criar_behavior_node("Botao_apertado", Botao_apertado_Node);
    botao_children[1] = criar_behavior_node("Expressao_risada", Expressao_risada_Node);
    Node* botao = criar_node(SEQUENCE, "Botao", tick_sequence, 2, botao_children, 0);

    Node** analogico_children = (Node**)malloc(sizeof(Node*) * 2);
    analogico_children[0] = criar_behavior_node("Direcional_dif_zero", Direcional_dif_zero_Node);
    analogico_children[1] = criar_behavior_node("Expressao_adaptativa", Expressao_adaptativa_Node);
    Node* analogico = criar_node(PARALLEL, "Analogico", tick_parallel, 2, analogico_children, 1);

    Node** human_children = (Node**)malloc(sizeof(Node*) * 2);
    human_children[0] = botao;
    human_children[1] = analogico;
    return criar_node(SELECTOR, "human", tick_selector, 2, human_children, 0);
}

Node* fallback_robo() {
    Node** sono_children = (Node**)malloc(sizeof(Node*) * 2);
    sono_children[0] = criar_behavior_node("Contagem_sono_maior_q_x", Contagem_sono_maior_q_x_Node);
    sono_children[1] = criar_behavior_node("Expressao_dormindo", Expressao_dormindo_Node);
    Node* sono = criar_node(SEQUENCE, "sono", tick_sequence, 2, sono_children, 0);

    Node** tedio_children = (Node**)malloc(sizeof(Node*) * 2);
    tedio_children[0] = criar_behavior_node("Contagem_tedio_maior_q_x", Contagem_tedio_maior_q_x_Node);
    tedio_children[1] = criar_behavior_node("Expressao_sono", Expressao_sono_Node);
    Node* tedio = criar_node(SEQUENCE, "tedio", tick_sequence, 2, tedio_children, 0);

    Node** sem_acao_children = (Node**)malloc(sizeof(Node*) * 2);
    sem_acao_children[0] = criar_behavior_node("Esta_x_tempo_sem_acao_externa", Esta_x_tempo_sem_acao_externa_Node);
    sem_acao_children[1] = criar_behavior_node("Expressao_tedio", Expressao_tedio_Node);
    Node* sem_acao = criar_node(SEQUENCE, "sem_acao", tick_sequence, 2, sem_acao_children, 0);

    Node** robo_children = (Node**)malloc(sizeof(Node*) * 3);
    robo_children[0] = sono;
    robo_children[1] = tedio;
    robo_children[2] = sem_acao;
    return criar_node(SELECTOR, "robo", tick_selector, 3, robo_children, 0);
}

Node* fallback_roboaleatorio() {
    Node** sapeca_children = (Node**)malloc(sizeof(Node*) * 2);
    sapeca_children[0] = criar_behavior_node("mexer_braco_para_cima_ebaixo", mexer_braco_para_cima_ebaixo_Node);
    sapeca_children[1] = criar_behavior_node("Expressao_sapeca", Expressao_sapeca_Node);
    Node* sapeca = criar_node(PARALLEL, "Paralelo movimento", tick_parallel, 2, sapeca_children, 1);

    Node** movimento_children = (Node**)malloc(sizeof(Node*) * 2);
    movimento_children[0] = criar_behavior_node("Valor_entre_x_e_x", Valor_entre_x_e_x_Node);
    movimento_children[1] = sapeca;
    Node* movimento = criar_node(SEQUENCE, "movimento", tick_sequence, 2, movimento_children, 0);

    Node** giro_children = (Node**)malloc(sizeof(Node*) * 2);
    giro_children[0] = criar_behavior_node("Mecher_braco_para_cima", Mecher_braco_para_cima_Node);
    giro_children[1] = criar_behavior_node("Expressao_alegre", Expressao_alegre_Node);
    Node* giro = criar_node(PARALLEL, "Paralelo giro", tick_parallel, 2, giro_children, 1);

    Node** girada_children = (Node**)malloc(sizeof(Node*) * 2);
    girada_children[0] = criar_behavior_node("Valor_foi_y", Valor_foi_y_Node);
    girada_children[1] = giro;
    Node* girada = criar_node(SEQUENCE, "girada", tick_sequence, 2, girada_children, 0);

    Node** aleatorio_children = (Node**)malloc(sizeof(Node*) * 3);
    aleatorio_children[0] = movimento;
    aleatorio_children[1] = girada;
    aleatorio_children[2] = criar_behavior_node("Expressao_Padrao", Expressao_Padrao_Node);
    return criar_node(SELECTOR, "aleatorio", tick_selector, 3, aleatorio_children, 0);
}

Node* loop() {
    Node** balanco_children = (Node**)malloc(sizeof(Node*) * 2);
    balanco_children[0] = criar_behavior_node("Esta_Sendo_Balancado", Esta_Sendo_Balancado_Node);
    balanco_children[1] = criar_behavior_node("Expressao_enjoo", Expressao_enjoo_Node);
    Node* balanco = criar_node(SEQUENCE, "Balanço", tick_sequence, 2, balanco_children, 0);

    Node** robo_aleatorio_children = (Node**)malloc(sizeof(Node*) * 2);
    robo_aleatorio_children[0] = criar_behavior_node("Valor_aleatorio", Valor_aleatorio_Node);
    robo_aleatorio_children[1] = fallback_roboaleatorio();
    Node* robo_aleatorio = criar_node(SEQUENCE, "robo_aleatorio", tick_sequence, 2, robo_aleatorio_children, 0);

    Node** fallback_acoes_children = (Node**)malloc(sizeof(Node*) * 3);
    fallback_acoes_children[0] = fallback_humano();
    fallback_acoes_children[1] = fallback_robo();
    fallback_acoes_children[2] = robo_aleatorio;
    Node* fallback_acoes = criar_node(SELECTOR, "fallback_acoes", tick_selector, 3, fallback_acoes_children, 0);

    Node** fallback_chao_children = (Node**)malloc(sizeof(Node*) * 2);
    fallback_chao_children[0] = balanco;
    fallback_chao_children[1] = criar_recuada();
    Node* fallback_chao = criar_node(SELECTOR, "fallback_chao", tick_selector, 2, fallback_chao_children, 0);

    Node** longe_chao_children = (Node**)malloc(sizeof(Node*) * 2);
    longe_chao_children[0] = criar_behavior_node("Dist_chao_maior_q_x", Dist_chao_maior_q_x_Node);
    longe_chao_children[1] = fallback_chao;
    Node* Longe_chao = criar_node(SEQUENCE, "Longe chao", tick_sequence, 2, longe_chao_children, 0);

    Node** desvio_parede_children = (Node**)malloc(sizeof(Node*) * 2);
    desvio_parede_children[0] = criar_behavior_node("Dist_Parede_menor_q_x", Dist_Parede_menor_q_x_Node);
    desvio_parede_children[1] = criar_recuada();
    Node* Desvio_parede = criar_node(SEQUENCE, "Desvio Parede", tick_sequence, 2, desvio_parede_children, 0);

    Node** loops_children = (Node**)malloc(sizeof(Node*) * 5);
    loops_children[0] = criar_behavior_node("Contagem_Tempo", Contagem_Tempo_Node);
    loops_children[1] = criar_behavior_node("Mostragem_Bateria", Mostragem_Bateria_Node);
    loops_children[2] = Desvio_parede;
    loops_children[3] = Longe_chao;
    loops_children[4] = fallback_acoes;
    return criar_node(PARALLEL, "Paralelo principal", tick_parallel, 5, loops_children, 1);
}

Node* build_behavior_tree() {
    Node** loop_node_children = (Node**)malloc(sizeof(Node*) * 2);
    loop_node_children[0] = loop();  // o loop de ações
    loop_node_children[1] = criar_behavior_node("Bateria_Fraca", Bateria_Fraca_Node); // nó de bateria

    Node* repeat_principal = criar_node(SEQUENCE, "Loop até bateria fraca", tick_decorator_repeat, 2, loop_node_children, 0);

    return repeat_principal;
}

// =========================================================================
//                             6. FUNÇÃO PRINCIPAL
// =========================================================================

int main() {
    stdio_init_all();
    
    Node* tree = build_behavior_tree();

    while(true) {
        printf("\n--- Novo Tick da Arvore ---\n");
        tree->tick_function(tree);
        sleep_ms(100);
    }

    return 0;
}