#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define FRAME_WIDTH 44   
#define FRAME_HEIGHT 48  
#define NUM_FRAMES 4     
#define FRAME_DURATION 0.2 
#define JUMP_HEIGHT 80  
#define GRAVITY 4     

int fase_atual = 0;
bool menu_aberto = false;
char jogo_ativo = 0;
bool perdeu_fase2 = true;

void desenhar_menu(ALLEGRO_FONT* fonte) {
    al_clear_to_color(al_map_rgb(0, 0, 0));


    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTER, "1. Fase 1");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "2. Fase 2");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "3. Fase 3");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 350, ALLEGRO_ALIGN_CENTER, "4. Voltar para casa");

    al_flip_display();
}
ALLEGRO_BITMAP* Blvazio;
ALLEGRO_BITMAP* Blsolido;
ALLEGRO_BITMAP* Bldestrutivel;
ALLEGRO_BITMAP* Objetivo;
ALLEGRO_BITMAP* persona;
const int linhas = 15;
const int colunas = 20;
int board[15][20];
bool sair = false;
bool sairBomber = true;
int inicialX = 1;
int inicialY = 1;

int Bomberman(int a) {
    float frame = 0;
    srand(time(NULL));
    Blvazio = al_load_bitmap("C:\\Users\\Leon\\source\\repos\\JogoEmAllegro\\JogoEmAllegro\\imagem-20241104T184339Z-001\\imagem\\vazio.png");
    persona = al_load_bitmap("C:\\Users\\Leon\\source\\repos\\JogoEmAllegro\\JogoEmAllegro\\imagem-20241104T184339Z-001\\imagem\\PGParado.png");
    Blsolido = al_load_bitmap("C:\\Users\\Leon\\source\\repos\\JogoEmAllegro\\JogoEmAllegro\\imagem-20241104T184339Z-001\\imagem\\parede.png");
    Bldestrutivel = al_load_bitmap("C:\\Users\\Leon\\source\\repos\\JogoEmAllegro\\JogoEmAllegro\\imagem-20241104T184339Z-001\\imagem\\arbusto.png");


    ALLEGRO_DISPLAY* bomber = al_create_display(800, 600);
    ALLEGRO_EVENT_QUEUE* fila_de_eventos = al_create_event_queue();
    al_register_event_source(fila_de_eventos, al_get_display_event_source(bomber));
    al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());

    // Inicialização do tabuleiro
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            int n1 = rand() % 10;
            board[i][j] = (n1 % 4 == 0) ? 1 : 0; // 1: Bloco sólido, 0: Espaço vazio
        }
    }
    for (int i = 0; i < linhas; i += 2) {
        for (int j = 0; j < colunas; j += 2) {
            if (board[i][j] == 0) board[i][j] = 2; // Bloco destrutível
        }
    }
    board[inicialY][inicialX] = 3;           // Posição inicial do jogador
    board[linhas - 1][colunas - 1] = 4;      // Objetivo

    while (!sairBomber) {
        // Desenha o tabuleiro
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                ALLEGRO_BITMAP* imagens = NULL;
                switch (board[i][j]) {
                case 0:
                    imagens = Blvazio;
                    break; // Espaço vazio

                case 1:
                    imagens = Blsolido;
                    break;       // Bloco sólido

                case 2:
                    imagens = Bldestrutivel;
                    break;     // Bloco destrutível
                case 3:
                    imagens = persona;
                    break;     // Jogador
                    // case 4: 
                       //  color = al_map_rgb(0, 255, 255); 
                      //   break;   // Objetivo
                }
                if (imagens) {
                    al_draw_bitmap(imagens, j * 40, i * 40, 0);
                }
            }
        }
        al_flip_display();
        frame += 0.3;
        if ((int)frame >= NUM_FRAMES) {
            frame = 0;
        }
        // Processamento de eventos
        ALLEGRO_EVENT event;
        al_wait_for_event(fila_de_eventos, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            int dx = 0, dy = 0;
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_RIGHT: dx = 1; break;
            case ALLEGRO_KEY_LEFT:  dx = -1; break;
            case ALLEGRO_KEY_UP:    dy = -1; break;
            case ALLEGRO_KEY_DOWN:  dy = 1; break;
            case ALLEGRO_KEY_SPACE: // Reinicializa o tabuleiro
                al_destroy_display(bomber);
                Bomberman(a);
                return 1;
            case ALLEGRO_KEY_ESCAPE: // Encerra o jogo
                sairBomber = true;
                break;
            }

            int novoX = inicialX + dx;
            int novoY = inicialY + dy;
            if (novoX >= 0 && novoX < colunas && novoY >= 0 && novoY < linhas &&
                (board[novoY][novoX] == 0 || board[novoY][novoX] == 2 || board[novoY][novoX] == 4)) {
                board[inicialY][inicialX] = 0;  // Atualiza a posição anterior do jogador
                inicialX = novoX;
                inicialY = novoY;
                board[inicialY][inicialX] = 3;  // Atualiza a nova posição do jogador

            }
        }
        if (board[inicialY][inicialX] == board[linhas - 14][colunas - 18]) {
            printf("Voce conseguiu");
            sairBomber = true;
            return 0;
        }
    }
    al_destroy_bitmap(Bldestrutivel);
    al_destroy_bitmap(Blsolido);
    al_destroy_bitmap(Blvazio);
    al_destroy_display(bomber);
    al_destroy_event_queue(fila_de_eventos);
}

int main() {
    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_install_mouse();
    ALLEGRO_FONT* fonte = al_create_builtin_font();

    ALLEGRO_DISPLAY* disp = al_create_display(800, 600);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();

    bool sair = false;
    bool on_homepage = true;
    float x = 30;
    float y = 435;
    float frame = 0;

    bool pulando = false;
    float jump_speed = 0;
    float jump_start_y = y;

    bool andando_esquerda = false;
    bool andando_direita = false;
    bool parado = true;

    int botaox = 280;
    int largura = 225;
    int botaoy = 360;
    int altura = 115;

    ALLEGRO_BITMAP* casa = al_load_bitmap("C:/Users/Leon/source/repos/JogoEmAllegro/JogoEmAllegro/casa.png");
    ALLEGRO_BITMAP* Personagem = al_load_bitmap("C:/Users/Leon/source/repos/JogoEmAllegro/JogoEmAllegro/Walking.png");
    ALLEGRO_BITMAP* Pulando = al_load_bitmap("C:/Users/Leon/source/repos/JogoEmAllegro/JogoEmAllegro/Pulando.png");
    ALLEGRO_BITMAP* Parado = al_load_bitmap("C:/Users/Leon/source/repos/JogoEmAllegro/JogoEmAllegro/PGParado.png");
    ALLEGRO_BITMAP* AndandoESQ = al_load_bitmap("C:/Users/Leon/source/repos/JogoEmAllegro/JogoEmAllegro/WalkingLeft.png");
    ALLEGRO_BITMAP* Homepage = al_load_bitmap("C:/Users/Leon/source/repos/JogoEmAllegro/JogoEmAllegro/bg2.jpg");
    ALLEGRO_BITMAP* cenario1 = al_load_bitmap("C:/Users/Leon/source/repos/JogoEmAllegro/JogoEmAllegro/cenario1.png");
    ALLEGRO_BITMAP* cenario2 = al_load_bitmap("C:/Users/Leon/source/repos/JogoEmAllegro/JogoEmAllegro/cenario2.png");
    ALLEGRO_BITMAP* cenario3 = al_load_bitmap("C:/Users/Leon/source/repos/JogoEmAllegro/JogoEmAllegro/cenario3.png");

    al_register_event_source(fila_eventos, al_get_display_event_source(disp));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());


    al_start_timer(timer);

    while (!sair) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);


        if (evento.type == ALLEGRO_EVENT_TIMER) {

            if (on_homepage) {

                al_draw_scaled_bitmap(Homepage, 0, 0, al_get_bitmap_width(Homepage), al_get_bitmap_height(Homepage), 0, 0, 800, 600, 0);
                al_flip_display();
            }
            else if (menu_aberto) {
                desenhar_menu(fonte);
            }
            else {

                ALLEGRO_BITMAP* cenario_atual;
                if (fase_atual == 1) {
                    cenario_atual = cenario1;
                }
                else if (fase_atual == 2) {
                    cenario_atual = casa;

                    if (perdeu_fase2) {
                        fase_atual = 0;
                        perdeu_fase2 = false;
                    }
                }
                else if (fase_atual == 3) {
                    cenario_atual = cenario3;
                }
                else {
                    cenario_atual = casa;
                }

                al_draw_scaled_bitmap(cenario_atual, 0, 0, al_get_bitmap_width(cenario_atual), al_get_bitmap_height(cenario_atual), 0, 0, 800, 600, 0);


                if (andando_esquerda) {
                    x -= 3;
                    parado = false;
                }
                if (andando_direita) {
                    x += 3;
                    parado = false;
                }

                if (!andando_esquerda && !andando_direita && !pulando) {
                    parado = true;
                }

                if (pulando) {
                    y -= jump_speed;
                    jump_speed -= GRAVITY * 0.1;

                    if (y >= jump_start_y) {
                        y = jump_start_y;
                        pulando = false;
                        jump_speed = 0;
                    }
                }


                frame += 0.3;
                if ((int)frame >= NUM_FRAMES) {
                    frame = 0;
                }

                if (pulando) {
                    al_draw_scaled_bitmap(Pulando, ((int)frame) * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                }
                else if (parado) {
                    al_draw_scaled_bitmap(Parado, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                }
                else if (andando_esquerda) {
                    al_draw_scaled_bitmap(AndandoESQ, ((int)frame) * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                }
                else if (andando_direita) {
                    al_draw_scaled_bitmap(Personagem, ((int)frame) * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                }

                al_flip_display();
            }
        }
        
            
                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    if (on_homepage && evento.mouse.button == 1) {
                        if (evento.mouse.x >= botaox && evento.mouse.x <= botaox + largura &&
                            evento.mouse.y >= botaoy && evento.mouse.y <= botaoy + altura) {
                            on_homepage = false;
                           
                        }
                    }
                }
                else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                    if (!on_homepage) {
                        switch (evento.keyboard.keycode) {
                        case ALLEGRO_KEY_LEFT:
                            andando_esquerda = true;
                            parado = false;
                            break;

                        case ALLEGRO_KEY_RIGHT:
                            andando_direita = true;
                            parado = false;
                            break;

                        case ALLEGRO_KEY_UP:
                            if (!pulando) {
                                pulando = true;
                                jump_speed = 10;
                                jump_start_y = y;
                            }
                            break;

                        case ALLEGRO_KEY_ESCAPE:

                            menu_aberto = !menu_aberto;
                            break;

                        case ALLEGRO_KEY_1:
                            if (menu_aberto) {
                                fase_atual = 1;
                                menu_aberto = false;
                            }
                            break;

                        case ALLEGRO_KEY_2:
                            if (menu_aberto) {
                                fase_atual = 2;
                                menu_aberto = false;
                                jogo_ativo = true;
                                system("C:\\Users\\Leon\\source\\repos\\spaceinvaders\\spaceinvaders\\x64\\Debug\\spaceinvaders.exe");
                                jogo_ativo = 1;

                            }
                            break;

                        case ALLEGRO_KEY_3:
                            if (menu_aberto) {
                                sairBomber = false;
                                int n = Bomberman(1);
                                if (n == 0) {
                                    menu_aberto = false;
                                    printf("\nMenuFechado");
                                }

                            }
                            break;

                        case ALLEGRO_KEY_4:
                            if (menu_aberto) {
                                fase_atual = 0;
                                menu_aberto = false;
                            }
                            break;

                        case ALLEGRO_KEY_5:
                            if (menu_aberto) {
                                fase_atual = -1;
                                menu_aberto = false;
                            }
                            break;

                        }
                    }
                }
             
        
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            switch (evento.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                andando_esquerda = false;
                break;

            case ALLEGRO_KEY_RIGHT:
                andando_direita = false;
                break;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = true;
        }
    }


    al_destroy_bitmap(Personagem);
    al_destroy_bitmap(casa);
    al_destroy_bitmap(Homepage);
    al_destroy_bitmap(cenario1);
    al_destroy_bitmap(cenario2);
    al_destroy_bitmap(cenario3);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);
    al_destroy_font(fonte);

    return 0;
}