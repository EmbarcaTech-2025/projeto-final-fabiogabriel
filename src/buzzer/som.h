#ifndef BUZZER_COMBINED_H
#define BUZZER_COMBINED_H

#include "pico/stdlib.h"

#define do 29886
#define do_2 28294
#define re 26795
#define re_2 25386
#define mi 24063

#define fa 22823
#define fa_2 21663
#define sol 20579
#define sol_2 19569
#define la 18630

#define la_2 17758
#define si 16952
#define DO 15963
#define DO_2 14929
#define RE 14097

#define RE_2 13298
#define MI 12503
#define FA 11794
#define FA_2 11157
#define SOL 10529

#define SOL_2 9950
#define LA 9332
#define LA_2 8790
#define SI 8296
#define PAUSE 1

#define SEMIBREVE      2000   // Semibreve (4 tempos)
#define SEMIBREVE_P    3000   // Semibreve pontuada (4 + 2 = 6 tempos)

#define MINIMA         1000   // Mínima (2 tempos)
#define MINIMA_P       1500   // Mínima pontuada (2 + 1 = 3 tempos)

#define SEMINIMA       500    // Semínima (1 tempo)
#define SEMINIMA_P     750    // Semínima pontuada (1 + 0.5 = 1.5 tempos)

#define COLCHEIA       250    // Colcheia (meio tempo)
#define COLCHEIA_P     375    // Colcheia pontuada (0.5 + 0.25 = 0.75 tempos)

#define SEMICOLCHEIA   125    // Semicolcheia (um quarto de meio tempo)
#define SEMICOLCHEIA_P 187    // Semicolcheia pontuada (0.25 + 0.125 = 0.375 tempos)

#define FUSA            62    // Fusa (1/8 de semicolcheia)
#define FUSA_P          93    // Fusa pontuada (1/8 + 1/16 = 3/16 tempos)


// Inicializa o PWM no pino do buzzer.
void pwm_init_buzzer(uint pin);

// Toca uma nota com a frequência e duração especificadas.
void play_tone(uint pin, uint frequency, uint duration_ms);

// Toca a música tema de Star Wars.
void play_star_wars_theme(uint pin);
void play_risada_sound(uint pin);
void play_angry_sound(uint pin);
void play_piscada_sound(uint pin);
void play_dizzy_sound(uint pin);
void play_medo_sound(uint pin);

// Constantes de som (extern para serem acessadas de outros arquivos).
extern const uint star_wars_notes[];
extern const uint star_wars_duration[];

#endif // BUZZER_COMBINED_H
