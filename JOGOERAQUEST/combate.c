
#include <stdio.h>
#include <stdbool.h>
#include "combate.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

static ALLEGRO_BITMAP* fundoCombate = NULL;
static ALLEGRO_FONT* fontePergunta = NULL;
static ALLEGRO_FONT* fonteResposta = NULL;

static int vidaNox = 100;
static int vidaTrex = 100;

static int perguntaAtual = 0;

static bool terminou = false;
static bool venceu = false;
static bool fase2Solicitada = false;

typedef struct {
    const char* pergunta;
    const char* alternativas[4];
    int correta;
} Pergunta;

static Pergunta perguntas[] = {
    {
        "Em qual periodo terminou a Era Mesozoica?",
        {"Triassico", "J Jurassico", "Cretaceo", "Permiano"},
        2
    },
    {
        "Qual cientista criou o nome Dinosauria, em 1842?",
        {"William Buckland", "Richard Owen", "Gideon Mantell", "Charles Darwin"},
        1
    }
};

#define TOTAL_PERGUNTAS 2

void iniciarCombate(void) {
    vidaNox = 100;
    vidaTrex = 100;
    perguntaAtual = 0;

    terminou = false;
    venceu = false;
    fase2Solicitada = false;

    fundoCombate = al_load_bitmap("boss.jpg");

    fontePergunta = al_load_ttf_font(
        "C:/Windows/Fonts/arialbd.ttf", 32, 0
    );

    fonteResposta = al_load_ttf_font(
        "C:/Windows/Fonts/arialbd.ttf", 25, 0
    );
}

void atualizarCombate(ALLEGRO_EVENT* evento) {

    if (evento->type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        return;

    if (evento->mouse.button != 1)
        return;

    // Se o combate terminou com vitória, verifica o botão da fase 2
    if (terminou) {

        if (venceu &&
            evento->mouse.x >= 760 && evento->mouse.x <= 1160 &&
            evento->mouse.y >= 530 && evento->mouse.y <= 610) {

            fase2Solicitada = true;
        }

        return;
    }

    

    int x = evento->mouse.x;
    int y = evento->mouse.y;

    int alternativa = -1;

    if (y >= 798 && y <= 895) {
        if (x >= 78 && x <= 948)
            alternativa = 0;
        else if (x >= 967 && x <= 1841)
            alternativa = 1;
    }

    else if (y >= 908 && y <= 1010) {
        if (x >= 78 && x <= 948)
            alternativa = 2;
        else if (x >= 967 && x <= 1841)
            alternativa = 3;
    }

    if (alternativa == -1)
        return;

    if (alternativa == perguntas[perguntaAtual].correta) {
        vidaTrex -= 50;

        if (vidaTrex <= 0) {
            vidaTrex = 0;
            terminou = true;
            venceu = true;
            return;
        }
    }
    else {
        vidaNox -= 50;

        if (vidaNox <= 0) {
            vidaNox = 0;
            terminou = true;
            venceu = false;
            return;
        }
    }

    perguntaAtual++;

    if (perguntaAtual >= TOTAL_PERGUNTAS) {
        terminou = true;
        venceu = (vidaTrex == 0);
    }
}

void desenharCombate(void) {

    if (fundoCombate) {
        al_draw_scaled_bitmap(
            fundoCombate,
            0, 0,
            al_get_bitmap_width(fundoCombate),
            al_get_bitmap_height(fundoCombate),
            0, 0, 1920, 1080, 0
        );
    }

    // Barras de vida
    al_draw_filled_rectangle(
        290, 118,
        290 + (354 * vidaNox / 100), 145,
        al_map_rgb(40, 190, 70)
    );

    al_draw_filled_rectangle(
        1506, 118,
        1506 + (336 * vidaTrex / 100), 145,
        al_map_rgb(210, 45, 45)
    );

    if (terminou) {

        const char* mensagem = venceu ?
            "VOCE VENCEU!" : "VOCE FOI DERROTADO!";

        if (fontePergunta) {
            al_draw_text(
                fontePergunta,
                al_map_rgb(255, 215, 0),
                960, 650,
                ALLEGRO_ALIGN_CENTER,
                mensagem
            );
        }

        if (venceu) {
            al_draw_filled_rectangle(
                760, 530, 1160, 610,
                al_map_rgb(40, 150, 60)
            );

            al_draw_rectangle(
                760, 530, 1160, 610,
                al_map_rgb(255, 255, 255), 3
            );

            al_draw_text(
                fontePergunta,
                al_map_rgb(255, 255, 255),
                960, 550,
                ALLEGRO_ALIGN_CENTER,
                "IR PARA FASE 2"
            );
        }

        return;
    }

    Pergunta p = perguntas[perguntaAtual];

    // Texto da pergunta: painel superior
    if (fontePergunta) {
        al_draw_text(
            fontePergunta,
            al_map_rgb(30, 30, 30),
            960, 680,
            ALLEGRO_ALIGN_CENTER,
            p.pergunta
        );
    }

    // Alternativas
    if (fontePergunta) {

        for (int i = 0; i < 4; i++) {

            int posX = (i % 2 == 0) ? 520 : 1400;
            int posY = (i < 2) ? 833 : 945;

            char texto[150];

            snprintf(texto, sizeof(texto), "%c) %s", 'A' + i, p.alternativas[i]);

            al_draw_text(
                fontePergunta,
                al_map_rgb(35, 30, 35),
                posX, posY,
                ALLEGRO_ALIGN_CENTER,
                texto
            );
        }
    }
}


bool combateTerminou(void) {
    return terminou;
}

bool jogadorVenceu(void) {
    return venceu;
}

bool solicitouFase2(void) {
    return fase2Solicitada;
}

void finalizarCombate(void) {

    if (fundoCombate)
        al_destroy_bitmap(fundoCombate);

    if (fontePergunta)
        al_destroy_font(fontePergunta);

    if (fonteResposta)
        al_destroy_font(fonteResposta);

    fundoCombate = NULL;
    fontePergunta = NULL;
    fonteResposta = NULL;
}
