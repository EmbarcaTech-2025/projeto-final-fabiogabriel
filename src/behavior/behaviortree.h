#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H

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
    NodeStatus status;
} Node;


// =========================================================================
//                     2. DECLARAÇÕES DE FUNÇÕES
// =========================================================================

// Funções de tick dos nós de composição
NodeStatus tick_sequence(Node* node);
NodeStatus tick_selector(Node* node);
NodeStatus tick_parallel(Node* node);
NodeStatus tick_decorator_repeat(Node* node);

// Funções para a criação da árvore
Node* criar_node(NodeType type, const char* name, NodeStatus (*tick_function)(Node*), int num_children, Node** children_array, int success_threshold);
Node* criar_behavior_node(const char* name, NodeStatus (*tick_function)(Node*));
Node* build_behavior_tree();


// =========================================================================
//                  DECLARAÇÕES DOS NÓS DE COMPORTAMENTO
// =========================================================================
// Estas funções são declaradas aqui para que os construtores da árvore possam acessá-las
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
NodeStatus Esta_na_açao_longe_chao(Node* node);
NodeStatus Retorna_failure(Node* node);
NodeStatus Expressao_lado(Node* node);

#endif