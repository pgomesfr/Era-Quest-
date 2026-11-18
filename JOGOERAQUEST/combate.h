
#ifndef COMBATE_H
#define COMBATE_H

#include <stdbool.h>
#include <allegro5/allegro.h>

void iniciarCombate(void);
void atualizarCombate(ALLEGRO_EVENT* evento);
void desenharCombate(void);
bool combateTerminou(void);
bool jogadorVenceu(void);
bool solicitouFase2(void);
void finalizarCombate(void);

#endif

