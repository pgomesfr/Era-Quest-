#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

typedef struct {
    float x, y, w, h;
} Plataforma;

static Plataforma plataformas[] = {
    {342, 385, 289, 85},
    {829, 310, 288, 85},
    {1272, 470, 264, 80},
    {503, 640, 295, 85},
    {1542, 640, 288, 85},
    {0, 1000, 1920, 145},
    {-100, -200, 100, 1480},
    {1920, -200, 100, 1480},
    {0, -100, 1920, 100}
};

#define TOTAL_PLATAFORMAS (int)(sizeof(plataformas) / sizeof(plataformas[0]))
#define PLATAFORMA_RELIQUIA 4
#define LARGURA_RELIQUIA 100
#define ALTURA_BOSS 650

static bool colide(float x, float y, float w, float h, Plataforma p) {
    return x + w > p.x && x < p.x + p.w &&
        y + h > p.y && y < p.y + p.h;
}

int main() {
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();

    ALLEGRO_DISPLAY* display = al_create_display(1920, 1080);
    al_set_window_title(display, "Era Quest");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(display));
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_mouse_event_source());
    al_register_event_source(fila, al_get_timer_event_source(timer));

    ALLEGRO_BITMAP* cenario = al_load_bitmap("cenario.png");
    ALLEGRO_BITMAP* nox = al_load_bitmap("nox.png");
    ALLEGRO_BITMAP* reliquia = al_load_bitmap("reliquia.png");
    ALLEGRO_BITMAP* dino = al_load_bitmap("dino.png");
    ALLEGRO_BITMAP* boss = al_load_bitmap("boss.png");

    ALLEGRO_FONT* fonte = al_load_ttf_font("C:/Windows/Fonts/arial.ttf", 30, 0);
    ALLEGRO_FONT* fonteTitulo = al_load_ttf_font("C:/Windows/Fonts/arialbd.ttf", 40, 0);

    float altura = al_get_bitmap_height(nox);
    float largura = al_get_bitmap_width(nox);
    float x = 200;
    float y = 1000;
    float velocidadex = 0;
    float velocidadey = 0;

    float reliquiaOriginalLargura = 0;
    float reliquiaOriginalAltura = 0;
    float reliquiaLargura = LARGURA_RELIQUIA;
    float reliquiaAltura = 0;
    float reliquiaX = 0;
    float reliquiaY = 0;
    bool reliquiaColetada = false;

    if (reliquia) {
        Plataforma base = plataformas[PLATAFORMA_RELIQUIA];
        reliquiaOriginalLargura = al_get_bitmap_width(reliquia);
        reliquiaOriginalAltura = al_get_bitmap_height(reliquia);
        reliquiaAltura = reliquiaOriginalAltura * (reliquiaLargura / reliquiaOriginalLargura);
        reliquiaX = base.x + (base.w - reliquiaLargura) / 2;
        reliquiaY = base.y - reliquiaAltura;
    }

    Plataforma portal = { 1600, 800, 190, 200 };

    float painelX = 360;
    float painelY = 190;
    float painelLargura = 1200;
    float painelAltura = 700;

    float botaoX = 1480;
    float botaoY = 205;
    float botaoTamanho = 60;

    bool mostrandoInfo = false;
    bool portalLiberado = false;
    bool naTelaBoss = false;

    float gravidade = 0.7;
    float velocidade = 6;
    float forcadopulo = -24;

    bool pisandonochao = false;
    bool olharparaesquerda = false;
    bool isRodando = true;

    al_start_timer(timer);

    while (isRodando) {
        ALLEGRO_EVENT evento;
        while (al_get_next_event(fila, &evento)) {
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                isRodando = false;
            }
            else if (evento.type == ALLEGRO_EVENT_KEY_DOWN &&
                evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                isRodando = false;
            }
            else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (mostrandoInfo &&
                    evento.mouse.x >= botaoX && evento.mouse.x <= botaoX + botaoTamanho &&
                    evento.mouse.y >= botaoY && evento.mouse.y <= botaoY + botaoTamanho) {
                    mostrandoInfo = false;
                    portalLiberado = true;
                }
            }
            else if (evento.type == ALLEGRO_EVENT_TIMER) {
                if (!mostrandoInfo && !naTelaBoss) {
                    ALLEGRO_KEYBOARD_STATE teclado;
                    al_get_keyboard_state(&teclado);

                    velocidadex = 0;
                    if (al_key_down(&teclado, ALLEGRO_KEY_LEFT)) {
                        velocidadex = -velocidade;
                        olharparaesquerda = true;
                    }
                    if (al_key_down(&teclado, ALLEGRO_KEY_RIGHT)) {
                        velocidadex = velocidade;
                        olharparaesquerda = false;
                    }

                    if (pisandonochao && al_key_down(&teclado, ALLEGRO_KEY_UP)) {
                        velocidadey = forcadopulo;
                    }

                    velocidadey += gravidade;

                    x += velocidadex;
                    for (int i = 0; i < TOTAL_PLATAFORMAS; i++) {
                        Plataforma p = plataformas[i];
                        if (colide(x, y, largura, altura, p)) {
                            if (velocidadex > 0) {
                                x = p.x - largura;
                            }
                            else if (velocidadex < 0) {
                                x = p.x + p.w;
                            }
                        }
                    }

                    y += velocidadey;
                    pisandonochao = false;
                    for (int i = 0; i < TOTAL_PLATAFORMAS; i++) {
                        Plataforma p = plataformas[i];
                        if (colide(x, y, largura, altura, p)) {
                            if (velocidadey > 0) {
                                y = p.y - altura;
                                pisandonochao = true;
                            }
                            else if (velocidadey < 0) {
                                y = p.y + p.h;
                            }
                            velocidadey = 0;
                        }
                    }

                    if (reliquia && !reliquiaColetada) {
                        Plataforma areaReliquia = { reliquiaX, reliquiaY, reliquiaLargura, reliquiaAltura };
                        if (colide(x, y, largura, altura, areaReliquia)) {
                            reliquiaColetada = true;
                            mostrandoInfo = true;
                        }
                    }

                    if (portalLiberado && colide(x, y, largura, altura, portal)) {
                        naTelaBoss = true;
                    }
                }
            }
        }

        if (naTelaBoss) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(fonteTitulo, al_map_rgb(255, 255, 255), 960, 40, ALLEGRO_ALIGN_CENTER, "Tela do Boss");

            if (boss) {
                al_draw_scaled_bitmap(boss,
                    0, 0, al_get_bitmap_width(boss), al_get_bitmap_height(boss),
                    0, 0, 1920, 1080, 0);
            }
        }
        else {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if (cenario) {
                al_draw_scaled_bitmap(cenario,
                    0, 0, al_get_bitmap_width(cenario), al_get_bitmap_height(cenario),
                    0, 0, 1920, 1080, 0);
            }

            if (reliquia && !reliquiaColetada) {
                al_draw_scaled_bitmap(reliquia,
                    0, 0, reliquiaOriginalLargura, reliquiaOriginalAltura,
                    reliquiaX, reliquiaY, reliquiaLargura, reliquiaAltura, 0);
            }

            if (nox) {
                al_draw_bitmap(nox, x, y, olharparaesquerda ? ALLEGRO_FLIP_HORIZONTAL : 0);
            }

            if (portalLiberado) {
                al_draw_text(fonteTitulo, al_map_rgb(255, 255, 255), 960, 40, ALLEGRO_ALIGN_CENTER, "Portal liberado! Vá até ele.");
            }

            if (mostrandoInfo) {
                al_draw_filled_rectangle(0, 0, 1920, 1080, al_map_rgba(0, 0, 0, 150));
                al_draw_filled_rectangle(painelX, painelY, painelX + painelLargura, painelY + painelAltura, al_map_rgb(30, 30, 50));
                al_draw_rectangle(painelX, painelY, painelX + painelLargura, painelY + painelAltura, al_map_rgb(255, 255, 255), 4);

                al_draw_text(fonteTitulo, al_map_rgb(255, 255, 255), painelX + 40, painelY + 20, 0, "Relíquia encontrada!");

                al_draw_text(fonte, al_map_rgb(255, 215, 0), painelX + 40, painelY + 110, 0,
                    "O fim dos dinossauros");
                al_draw_multiline_text(fonte, al_map_rgb(255, 255, 255), painelX + 40, painelY + 155, 1100, 36, 0,
                    "Os dinossauros viveram na Era Mesozoica. Essa era terminou no período Cretáceo, há cerca de 66 milhões de anos, quando um meteoro gigante atingiu a Terra onde hoje fica o México (cratera de Chicxulub). O impacto causou incêndios, nuvens de poeira e um grande esfriamento do clima, e a maioria dos dinossauros foi extinta.");

                al_draw_text(fonte, al_map_rgb(255, 215, 0), painelX + 40, painelY + 385, 0,
                    "A descoberta dos dinossauros");
                al_draw_multiline_text(fonte, al_map_rgb(255, 255, 255), painelX + 40, painelY + 430, 1100, 36, 0,
                    "Fósseis desses animais já existiam, mas só no início do século XIX os cientistas entenderam o que eram. Em 1824, o inglês William Buckland descreveu o Megalosaurus e, em 1825, Gideon Mantell descreveu o Iguanodon. Em 1842, o cientista Richard Owen criou o nome Dinosauria, que significa lagartos terríveis.");

                al_draw_text(fonte, al_map_rgb(180, 180, 180), painelX + 40, painelY + 640, 0,
                    "Leia com atenção: essas informações serão cobradas no boss. Clique no X para fechar.");

                al_draw_filled_rectangle(botaoX, botaoY, botaoX + botaoTamanho, botaoY + botaoTamanho, al_map_rgb(200, 30, 30));
                al_draw_text(fonteTitulo, al_map_rgb(255, 255, 255), botaoX + botaoTamanho / 2, botaoY + 8, ALLEGRO_ALIGN_CENTER, "X");
            }
        }

        al_flip_display();
    }

    if (cenario) al_destroy_bitmap(cenario);
    if (nox) al_destroy_bitmap(nox);
    if (reliquia) al_destroy_bitmap(reliquia);
    if (dino) al_destroy_bitmap(dino);
    if (boss) al_destroy_bitmap(boss);
    if (fonte) al_destroy_font(fonte);
    if (fonteTitulo) al_destroy_font(fonteTitulo);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    al_destroy_display(display);

    return 0;
}