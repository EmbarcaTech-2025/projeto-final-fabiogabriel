#include <stdio.h>
#include "behaviortree.h"
#include "distancia/sensor_distancia.h" // Inclua a biblioteca do sensor
#include "display/display.h"

// Declaração do sensor global para que este arquivo possa acessá-lo
extern VL53L0X sensor;

// =========================================================================
//                 IMPLEMENTAÇÃO DOS NÓS DE COMPORTAMENTO
// =========================================================================

NodeStatus Bateria_Fraca_Node(Node* node) {
    static int execucoes = 0;
    execucoes++;
    printf("Executando: %s (vez %d)\n", node->name, execucoes);
    if (execucoes >= 200) {
        return FAILURE;
    } else {
        return FAILURE;
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
    draw_fear_expression_tick();
    return SUCCESS;
}
NodeStatus Vire_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}

NodeStatus Dist_chao_maior_q_x_Node(Node* node) {
    // O valor 'x' é o limite em milímetros
    const int DISTANCIA_LIMITE_MM = 200; //  200mm = 20cm

    // Lê a distância do sensor
    int distancia_atual = sensor.readRangeContinuousMillimeters();

    printf("Executando: %s (Distancia: %d mm)\n", node->name, distancia_atual);

    // Verifica se o sensor está lendo uma distância válida
    if (distancia_atual == 65535) { 
        printf(" -> Erro na leitura do sensor!\n");
        return FAILURE;
    }
    
    // Retorna SUCCESS se a distância for maior que o limite
    if (distancia_atual > DISTANCIA_LIMITE_MM) {
        printf(" -> Distancia maior que %d mm. SUCESSO!\n", DISTANCIA_LIMITE_MM);
        return SUCCESS;
    } else {
        printf(" -> Distancia menor ou igual a %d mm. FALHA!\n", DISTANCIA_LIMITE_MM);
        return FAILURE;
    }
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