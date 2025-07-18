# Etapa 1 – Definição de Requisitos e Lista de Materiais

## Título do Projeto
**I-C: "Interactive Companion"**

## Descrição do Problema

Em um cenário onde a interação humana com a tecnologia se torna cada vez mais presente e personalizada, existe uma crescente demanda por dispositivos que não apenas executem tarefas, mas também enriqueçam o ambiente e proporcionem uma experiência mais envolvente e companheira.

Robôs com capacidade de interação social e autonomia de movimento, como os vistos em mídias populares, demonstram o potencial de preencher lacunas em diversas áreas, desde a companhia até o suporte em atividades diárias. No entanto, a criação de tais sistemas, que exijam comportamentos reativos, adaptativos e "com personalidade", apresenta desafios significativos em termos de arquitetura de software e integração de hardware.

O principal objetivo deste projeto é desenvolver um robô autônomo e interativo chamado **I-C**, capaz de se locomover e explorar ambientes internos, interagir com usuários, e exibir comportamentos que simulem "personalidade" por meio de expressões visuais e sonoras.

### Público-alvo

- Entusiastas de robótica e estudantes interessados em Behavior Trees para sistemas embarcados.
- Indivíduos em ambientes domésticos ou educacionais que buscam uma experiência interativa e lúdica com um robô companheiro.

### Valor Agregado

- Plataforma tangível e divertida para estudo e experimentação de conceitos avançados de robótica, como navegação autônoma e tomada de decisão baseada em Behavior Trees.
- Experiência de companhia e entretenimento através de interações com o ambiente.
- Estímulo à criatividade e aprendizado prático em eletrônica, programação e IA.

---

## Requisitos Funcionais

- **Mobilidade:** Locomoção autônoma em ambientes internos, desviando de obstáculos.
- **Interação Visual:** Exibição de "olhos" e expressões visuais no display LCD. Mensagens curtas no display OLED (ex: "Olá!", "Estou com sono...", "Bateria baixa").
- **Interação Sonora:** Emissão de sons (efeitos) e, opcionalmente, captação de comandos de voz simples.
- **Reação ao Ambiente:** Reação a estímulos como proximidade, luz e toque (se houver sensores de toque).
- **Comportamentos de Companhia:** Interação lúdica com o ambiente, "brincar" (movimentos e sons), e modo de "descanso".
- **Gerenciamento de Energia:** Monitoramento do nível da bateria e retorno à "base" para recarregar.

## Requisitos Não Funcionais

- **Autonomia:** Operação por um período mínimo de X horas com uma carga de bateria.
- **Segurança:** Navegação evitando colisões com pessoas e objetos.
- **Responsividade:** Reação a estímulos e comandos em tempo aceitável (ex: menos de Y segundos).
- **Expressividade:** Animações no LCD devem transmitir emoções de forma clara.

---

## Arquitetura Proposta

> A BitDogLab será o cérebro do robô, processando dados dos sensores e controlando os atuadores. As Behavior Trees serão a camada de inteligência artificial responsável por gerenciar todos os comportamentos do robô – desde a navegação autônoma e desvio de obstáculos até as interações sociais complexas, expressões emocionais e manutenção interna.

---

## Lista de Componentes Iniciais

- **BitDogLab**
- **Periféricos do Kit Embarcatech:**
  - Sensor i2c: Medição de distância a Laser
  - Display LCD 320x240 pixels (SPI)
  - Sensor i2c: Acelerômetro
  - Sensor i2c: Luminosidade
  - Periférico de extensão de conexões para sensores (i2c)
- **Componentes Adicionais (mobilidade):**
  - Chassi: Estrutura para o robô
  - 2x Motores DC com Encoders: Para as rodas do robô
  - 1x Driver de Motor (Ex: L298N ou similar): Para controlar os motores DC
  - 2x LiPo 18650
  - 1x Suporte para 2x18650
  - 1x Módulo Carregador de Bateria: Para a LiPo
  - 1x Roda Boba