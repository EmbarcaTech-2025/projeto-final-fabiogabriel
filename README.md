# Projeto Final - Interactive Companion (Robozinho I-C)

Autor: **Fábio Gabriel da Silva Barbosa**

Curso: Residência Tecnológica em Sistemas Embarcados

link para o código do projeto : https://github.com/EmbarcaTech-2025/projeto-final-fabiogabriel/tree/master

Instituição: EmbarcaTech - HBr

Brasília, julho de 2025

---

## Visão Geral do Projeto

O Interactive Companion é um projeto de robótica que vai além dos comandos básicos. Utilizando uma Árvore de Comportamento (Behavior Tree), ele é capaz de demonstrar "emoções" e reagir de forma dinâmica ao seu ambiente e ao toque do usuário. O objetivo é transformar um simples dispositivo em um companheiro interativo, com uma personalidade própria.
Funcionalidades Principais

## Demonstração em Vídeo

Para ver o Interactive Companion em ação, assista ao vídeo abaixo. 

![Demonstração das expressões animadas](Etapa_4/I-C-video.gif)

## Comportamento Dinâmico

A arquitetura da árvore de comportamento permite que o robô tome decisões complexas com base em diferentes estímulos.

### Detecção de Emoções:

Medo: Reage quando é levantado do chão ou quando detecta um movimento brusco (usando o sensor de proximidade e o MPU6050).

Risada: Reage com sons de risada quando o botão  é pressionado.

Tédio e Outras Expressões: A árvore pode ser expandida para incluir comportamentos como tédio ou surpresa.

### Interação Visual: 

Utiliza um display LCD(SPI) para exibir expressões faciais dinâmicas que seguem a posição do joystick.

### Feedback Sonoro: 

O projeto usa um buzzer PWM para reproduzir diferentes sons que expressam o estado emocional do robô. O volume pode ser ajustado facilmente no código.


### Movimento de Braço: 
A integração com o controlador de servo PCA9685 permite o controle preciso do braço do robô.

## Requisitos de Hardware

O projeto foi desenvolvido para ser executado com o seguinte hardware:

Microcontrolador: Raspberry Pi Pico

Placa de Expansão: Kit BitDogLab

    Sensores:

        Sensor VL53L0X (Distancia laser)

        Sensor MPU6050 (Acelerômetro e Giroscópio)

    Atuadores:

        Buzzer PWM

        Servo Motor

        Controlador de Servo PCA9685

    Display: display LCD 320x240 pixels (SPI)

## Modo de Uso

Para compilar e executar o projeto, siga os passos abaixo:

    Instale o SDK do Pico: Certifique-se de que o ambiente de desenvolvimento para o Raspberry Pi Pico esteja configurado.

    Clone o Repositório: Clone este repositório para sua máquina local.

    Compile o Código: Use o CMake para compilar o projeto em um arquivo .uf2.

    Faça o Upload: Carregue o arquivo .uf2 para o seu Raspberry Pi Pico.
