#include <stdio.h>
#include "behavior/behaviortree.h"
#include "distancia/sensor_distancia.h" // Inclua a biblioteca do sensor
#include "display/display.h"
#include "botoes/gpio.h" 
#include "joystick/joystick.h"

int medo_em_uso;
int direcional;
int contagem_tempo = 0;
int contagem_tedio;


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
    contagem_tempo++;

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
        limpar_tela();
        printf(" -> Distancia maior que %d mm. SUCESSO!\n", DISTANCIA_LIMITE_MM);
        medo_em_uso = 1;
        return SUCCESS;
    } else {
        medo_em_uso = 0;
        printf(" -> Distancia menor ou igual a %d mm. FALHA!\n", DISTANCIA_LIMITE_MM);
        //draw_normal_expression_tick();
        return FAILURE;
    }
}
NodeStatus Esta_na_açao_longe_chao(Node* node){
    if (medo_em_uso == 1){
        contagem_tempo = 0;
        return RUNNING;
    }
    else{
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

    if (!gpio_get(BUTTON_A) or !gpio_get(BUTTON_B)) {
        printf(" -> Botão A foi pressionado! SUCESSO.\n");
        contagem_tempo = 0;
        return SUCCESS;
    }

    printf(" -> Botão A não foi pressionado. FALHA.\n");
    //medo_em_uso = 0;
    return FAILURE;
}
NodeStatus Expressao_risada_Node(Node* node) {
    printf("Executando: %s\n", node->name);

    // Variável estática para manter o estado entre as execuções
    static int counter = 0;
    const int DURATION_TICKS = 2; // Tempo que a expressão de risada será exibida (50 ticks da arvore)

    
    //medo_em_uso = 1; // Inicia a expressão, marca como "em uso"
    

    draw_laugh_expression_tick();
    
    if (counter < DURATION_TICKS) {
        counter++;
        return RUNNING; // Retorna RUNNING para continuar exibindo a expressão
    } else {
        counter = 0; // Reseta o contador
         // Terminou a expressão, marca como "não em uso"
        return SUCCESS; // Retorna SUCCESS depois que o tempo passou
    }
}
NodeStatus Direcional_dif_zero_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    
    joystick_state_t joystick = read_joystick_state();
    
    const int DEADZONE = 100;
    const int CENTER = 2047;

    if ((joystick.x_value > CENTER + DEADZONE) || (joystick.x_value < CENTER - DEADZONE) ||
        (joystick.y_value > CENTER + DEADZONE) || (joystick.y_value < CENTER - DEADZONE)) {
        
        if (medo_em_uso == 1){
            printf("Ele esta longe do chao\n");
            return SUCCESS;
        }    
        printf(" -> Joystick movido! Retornando RUNNING.\n");
        
        contagem_tempo = 0;
        direcional = 1;
        return RUNNING;
    }

  
    direcional = 0;
    printf(" -> Joystick no centro. Retornando SUCCESS.\n");
    return SUCCESS;
}
NodeStatus Expressao_adaptativa_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    joystick_state_t joystick = read_joystick_state();

    draw_eyes_follow_joystick(joystick.x_value, joystick.y_value);

    return RUNNING;
}
NodeStatus Contagem_sono_maior_q_x_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return FAILURE;
}
NodeStatus Expressao_dormindo_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    draw_sleep_expression_tick();
    return SUCCESS;
}
NodeStatus Contagem_tedio_maior_q_x_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    if (contagem_tedio >= 5){
        contagem_tedio = 0;
        return SUCCESS;
    }
    return FAILURE;
}
NodeStatus Expressao_sono_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    draw_sleep_expression_tick();
    if (medo_em_uso == 1){
        return FAILURE;
    }
    return RUNNING;
}
NodeStatus Esta_x_tempo_sem_acao_externa_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    if (contagem_tempo >= 50){
        contagem_tedio++;
        contagem_tempo = 0;
        return SUCCESS;
    }

    return FAILURE;
}
NodeStatus Expressao_tedio_Node(Node* node) {
    printf("Executando: %s\n", node->name);

    if (direcional == 1){
        return FAILURE;
    }
    // Chama a função de animação de tédio
    draw_boredom_arc_eyes();

    return SUCCESS; // A animação está completa
    
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

    draw_normal_expression_tick();
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

NodeStatus Retorna_failure(Node* node){
    printf("Executando: %s\n", node->name);
    return FAILURE;
}