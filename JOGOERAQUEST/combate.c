
#include <stdio.h>
#include <stdbool.h>
#include "combate.h"

// Estado do combate
static int vidaJogador;
static int vidaChefe;
static bool terminou;
static bool venceu;

void iniciarCombate(void) {
    vidaJogador = 100;
    vidaChefe = 100;
    terminou = false;
    venceu = false;
}

void atualizarCombate(ALLEGRO_EVENT* evento) {
    // Vamos implementar os cliques nas respostas aqui.
}

void desenharCombate(void) {
    // Vamos implementar a interface do combate aqui.
}

bool combateTerminou(void) {
    return terminou;
}

bool jogadorVenceu(void) {
    return venceu;
}

void finalizarCombate(void) {
    // Liberação de recursos será implementada depois.
}
