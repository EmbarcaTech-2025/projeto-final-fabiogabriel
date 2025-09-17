#include <stdio.h>
#include "behavior/behaviortree.h"
#include "distancia/sensor_distancia.h" // Inclua a biblioteca do sensor
#include "display/display.h"
#include "botoes/gpio.h" 
#include "joystick/joystick.h"
#include "servo/pca9685.h"
#include "hardware/i2c.h"
#include "giroscópio/mpu6050.h"
#include "buzzer/som.h"

#define MPU6050_ADDRESS 0x68
#define I2C_PORT i2c1
#define I2C_SDA 2
#define I2C_SCL 3
#define PCA9685_ADDR 0x40
#define TICKS_TO_RUN 4
#define BUZZER_PIN 21

#define SHAKE_THRESHOLD 17000

int medo_em_uso;
int direcional;
int contagem_tempo = 0;
int contagem_tedio;
int acao_humana_uso;
int botao_B = 0;
int botao_A = 0;
static int random_value;


// Declaração do sensor global para que este arquivo possa acessá-lo
extern VL53L0X sensor;
extern volatile bool medo_ativo;


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
        play_medo_sound(BUZZER_PIN);
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

    // Variáveis para armazenar os dados do MPU6050
    int16_t acceleration[3], gyro[3], temp;

    // A porta I2C deve ser inicializada na função main antes de ser usada aqui.
    // Lendo os dados brutos do acelerômetro
    mpu6050_read_raw(I2C_PORT, acceleration, gyro, &temp);

    // Verifica se a aceleração em qualquer eixo (X, Y ou Z) ultrapassa o limite
    if (abs(acceleration[0]) > SHAKE_THRESHOLD ||
        abs(acceleration[1]) > SHAKE_THRESHOLD ||
        abs(acceleration[2]) > SHAKE_THRESHOLD) {
        printf("Detectado balanço! Aceleração X: %d, Y: %d, Z: %d\n",
               acceleration[0], acceleration[1], acceleration[2]);
        return SUCCESS; // Retorna sucesso se balançado
    } else {
        printf("Nao detectado balanço. Aceleração X: %d, Y: %d, Z: %d\n",
               acceleration[0], acceleration[1], acceleration[2]);
        return FAILURE; // Retorna falha se não for balançado
    }
}

NodeStatus Expressao_enjoo_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    draw_dizzy_expression_tick();
    play_dizzy_sound(BUZZER_PIN);
    return SUCCESS;
}
NodeStatus Expressao_raiva_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}
NodeStatus Botao_apertado_Node(Node* node) {
    printf("Executando: %s\n", node->name);

    if (!gpio_get(BUTTON_A)) {
        printf(" -> Botão A foi pressionado! SUCESSO.\n");
        contagem_tempo = 0;
        botao_A++;
        return SUCCESS;
    }
    else if (!gpio_get(BUTTON_B)){
        printf(" -> Botão B foi pressionado! SUCESSO.\n");
        contagem_tempo = 0;
        botao_B++;
        return SUCCESS;
    }

    printf(" -> Botão A não foi pressionado. FALHA.\n");
    //medo_em_uso = 0;
    return FAILURE;
}
NodeStatus Expressao_risada_Node(Node* node) {
    printf("Executando: %s\n", node->name);

    // Variáveis estáticas para manter o estado e a expressão ativa
    static int counter = 0;
    static int active_expression = 0; // 0 para risada, 1 para raiva
    const int DURATION_TICKS = 2;

    // Se o botão de raiva foi pressionado, mude a expressão ativa
    if (botao_A >= 3 && botao_A <= 4) {
        active_expression = 2; // Ativa a expressão de raiva

    }
    else if (botao_B >= 3 && botao_B <= 4) {
        active_expression = 2; // Ativa a expressão de raiva

    }
    else if (botao_A > 4 || botao_B > 4 )
    {
        active_expression = 1; 
        // Reseta o botão para que a condição não se repita
        botao_B = 0;
        botao_A = 0;
    }
    
    
    // Escolhe qual expressão desenhar com base no estado salvo
    if (active_expression == 1) {
        draw_angry_expression_tick();
        play_angry_sound(BUZZER_PIN);
    } 
    else if (active_expression == 2)
    {
        draw_laugh_expression_tick();
        play_risada_sound(BUZZER_PIN);
    }
    else {
        draw_sapeca_expression();
        play_risada_sound(BUZZER_PIN);
    }
    
    // Lógica para controlar a duração do nó
    if (counter < DURATION_TICKS) {
        counter++;
        return RUNNING; // Retorna RUNNING para continuar exibindo a expressão
    } else {
        counter = 0; // Reseta o contador
        active_expression = 0; // Volta para a expressão de risada por padrão
        acao_humana_uso = 0;
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
    if (joystick.button_pressed) {
        printf(" -> Botão do joystick foi pressionado! Tocando música.\n");
        draw_laugh_expression_tick();
        play_star_wars_theme(BUZZER_PIN); 
        
        return SUCCESS;
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
    if (contagem_tedio >= 10){
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
    else if (!gpio_get(BUTTON_A) or !gpio_get(BUTTON_B)) {
        printf(" -> Botão A foi pressionado! SUCESSO.\n");
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
    play_angry_sound(BUZZER_PIN);

    return SUCCESS; // A animação está completa
    
}
NodeStatus Valor_aleatorio_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    // Gera um valor aleatório entre 1 e 3
    random_value = (rand() % 7) + 1;
    printf(" -> Valor aleatório gerado: %d\n", random_value);
    return SUCCESS;
}
NodeStatus Valor_entre_x_e_x_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    if (random_value == 1){

        return SUCCESS;
    }
    else{
        return FAILURE;
    }
}
    
NodeStatus mexer_braco_para_cima_ebaixo_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    return SUCCESS;
}
NodeStatus Expressao_sapeca_Node(Node* node) {
    printf("Executando: %s\n", node->name);

    // Variável estática para manter o estado entre as execuções
    static int counter = 0;
    const int DURATION_TICKS = 2; // Tempo que a expressão de risada será exibida (50 ticks da arvore)
    
    draw_sapeca_expression();

    
    if (counter < DURATION_TICKS) {
        counter++;
        return RUNNING; // Retorna RUNNING para continuar exibindo a expressão
    } else {
        counter = 0; // Reseta o contador
        return SUCCESS; // Retorna SUCCESS depois que o tempo passou
    }
}
NodeStatus Valor_foi_y_Node(Node* node) {
    printf("Executando: %s\n", node->name);
    if (random_value == 2){

        return SUCCESS;
    }
    else{
        return FAILURE;
    }
    return FAILURE;
}
NodeStatus Expressao_lado(Node* node) {
    printf("Executando: %s\n", node->name);

    // Variável estática para manter o estado entre as execuções
    static int counter = 0;
    const int DURATION_TICKS = 2; // Tempo que a expressão de risada será exibida (50 ticks da arvore)
    
    draw_look_around_expression_tick();

    
    if (counter < DURATION_TICKS) {
        counter++;
        return RUNNING; // Retorna RUNNING para continuar exibindo a expressão
    } else {
        counter = 0; // Reseta o contador
        return SUCCESS; // Retorna SUCCESS depois que o tempo passou
    }
    return SUCCESS;
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