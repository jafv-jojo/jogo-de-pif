
#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "jogo.h"
#include "palavras.h"
#include "ranking.h"
#include "dueto.h"

#define LARGURA 600
#define ALTURA 700

#define TELA_MENU 0
#define TELA_TERMO 1
#define TELA_DUETO 2

void desenharMenu(void) {
    DrawText("JOGO DE PIF", 155, 90, 40, DARKGRAY);
    DrawText("Escolha o modo de jogo", 155, 155, 22, GRAY);

    DrawRectangle(150, 240, 300, 75, DARKGREEN);
    DrawText("TERMO", 245, 255, 30, WHITE);

    DrawRectangle(150, 340, 300, 75, GRAY);
    DrawText("DUETO", 245, 355, 30, WHITE);

    DrawRectangle(150, 440, 300, 75, GRAY);
    DrawText("QUARTETO", 210, 455, 30, WHITE);

    DrawText("Quarteto em desenvolvimento",
             145, 560, 18, DARKGRAY);
}

void desenharDueto(Dueto *dueto, char entrada[]) {
    int tamanhoCelula = 43;
    int espaco = 5;
    int inicioY = 145;

    for (int tab = 0; tab < DUETO_QUANTIDADE_PALAVRAS; tab++) {
        int inicioX = 45 + tab * 270;

        DrawText(TextFormat("PALAVRA %d", tab + 1),
                 inicioX + 55, 100, 20, DARKGRAY);

        for (int i = 0; i < DUETO_MAX_TENTATIVAS; i++) {
            for (int j = 0; j < DUETO_TAMANHO; j++) {
                int x = inicioX + j * (tamanhoCelula + espaco);
                int y = inicioY + i * (tamanhoCelula + espaco);
                Color cor = LIGHTGRAY;

                if (i < dueto->quantidadeTentativas &&
                    (!dueto->acertou[tab] ||
                     i < dueto->tentativaAcerto[tab])) {
                    if (dueto->cores[tab][i][j] == 2) {
                        cor = GREEN;
                    } else if (dueto->cores[tab][i][j] == 1) {
                        cor = GOLD;
                    } else {
                        cor = GRAY;
                    }
                }

                DrawRectangle(x, y, tamanhoCelula, tamanhoCelula, cor);
                DrawRectangleLines(x, y, tamanhoCelula,
                                   tamanhoCelula, DARKGRAY);

                char letra[2] = {'\0', '\0'};

                if (i < dueto->quantidadeTentativas &&
                    (!dueto->acertou[tab] ||
                     i < dueto->tentativaAcerto[tab])) {
                    letra[0] = dueto->tentativas[i][j];
                } else if (i == dueto->quantidadeTentativas &&
                           strlen(entrada) > (size_t)j &&
                           !dueto->acertou[tab]) {
                    letra[0] = entrada[j];
                }

                if (letra[0] != '\0') {
                    DrawText(letra, x + 12, y + 8, 27, WHITE);
                }
            }
        }

        if (dueto->acertou[tab]) {
            DrawText("ACERTOU!", inicioX + 65, 510, 20, GREEN);
        }
    }
}

void desenharJogo(Jogo *jogo, char entrada[]) {
    int inicioX = 150;
    int inicioY = 150;
    int tamanhoCelula = 55;
    int espaco = 7;

    for (int i = 0; i < MAX_TENTATIVAS; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            int x = inicioX + j * (tamanhoCelula + espaco);
            int y = inicioY + i * (tamanhoCelula + espaco);

            Color cor = LIGHTGRAY;

            if (i < jogo->quantidadeTentativas) {
                if (jogo->cores[i][j] == VERDE) {
                    cor = GREEN;
                } else if (jogo->cores[i][j] == AMARELO) {
                    cor = GOLD;
                } else {
                    cor = GRAY;
                }
            }

            DrawRectangle(x, y, tamanhoCelula, tamanhoCelula, cor);
            DrawRectangleLines(x, y, tamanhoCelula, tamanhoCelula, DARKGRAY);

            char letra[2] = {'\0', '\0'};

            if (i < jogo->quantidadeTentativas) {
                letra[0] = jogo->tentativas[i][j];
            } else if (i == jogo->quantidadeTentativas &&
                       strlen(entrada) > (size_t)j) {
                letra[0] = entrada[j];
            }

            if (letra[0] != '\0') {
                DrawText(letra, x + 17, y + 10, 35, WHITE);
            }
        }
    }
}

int main(void) {
    int telaAtual = TELA_MENU;

    if (!inicializarAPI()) {
        printf("Erro ao inicializar a API.\n");
        return 1;
    }

    char palavra[TAMANHO + 1];

    if (!escolherPalavra(palavra)) {
        printf("Erro ao obter palavra da API.\n");
        finalizarAPI();
        return 1;
    }

    Resultado *ranking = NULL;
    carregarRanking(&ranking, "ranking.txt");

    Jogo jogo;
    char entrada[TAMANHO + 1] = "";
    char mensagem[64] = "";

    iniciarJogo(&jogo, palavra);

    Dueto dueto;
    char palavraDueto1[DUETO_TAMANHO + 1];
    char palavraDueto2[DUETO_TAMANHO + 1];

    if (!escolherPalavra(palavraDueto1)) {
        printf("Erro ao obter a primeira palavra do Dueto.\n");
        finalizarAPI();
        liberarRanking(ranking);
        return 1;
    }

    int palavraDiferente = 0;

    for (int tentativa = 0; tentativa < 10; tentativa++) {
        if (escolherPalavra(palavraDueto2) &&
            strcmp(palavraDueto1, palavraDueto2) != 0) {
            palavraDiferente = 1;
            break;
        }
    }

    if (!palavraDiferente) {
        printf("Erro ao obter duas palavras diferentes.\n");
        finalizarAPI();
        liberarRanking(ranking);
        return 1;
    }

    iniciarDueto(&dueto, palavraDueto1, palavraDueto2);

    int registroSalvo = 0;

    InitWindow(LARGURA, ALTURA, "Termo");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (telaAtual == TELA_MENU) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse = GetMousePosition();

                if (CheckCollisionPointRec(
                    mouse, (Rectangle){150, 240, 300, 75})) {
                    telaAtual = TELA_TERMO;
                }

                if (CheckCollisionPointRec(
                    mouse, (Rectangle){150, 340, 300, 75})) {
                    telaAtual = TELA_DUETO;
                    entrada[0] = '\0';
                }
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);
            desenharMenu();
            EndDrawing();

            continue;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            telaAtual = TELA_MENU;
            entrada[0] = '\0';
            continue;
        }

        if (telaAtual == TELA_DUETO) {
            if (!dueto.terminou) {
                int tecla = GetCharPressed();

                while (tecla > 0) {
                    if (tecla >= 'a' && tecla <= 'z') {
                        tecla = toupper(tecla);
                    }

                    if (tecla >= 'A' && tecla <= 'Z' &&
                        strlen(entrada) < DUETO_TAMANHO) {
                        
                        mensagem[0] = '\0'; // msg do jogo desaparece qnd jogador comeca a digitar
                        int tamanho = (int)strlen(entrada);
                        entrada[tamanho] = (char)tecla;
                        entrada[tamanho + 1] = '\0';
                    }

                    tecla = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    int tamanho = (int)strlen(entrada);

                    if (tamanho > 0) {
                        entrada[tamanho - 1] = '\0';
                    }
                }

                if (IsKeyPressed(KEY_ENTER) &&
                    strlen(entrada) == DUETO_TAMANHO) {

                    int resultadoValidacao = validar_palavra(entrada);

                    if (resultadoValidacao == 1) {
                        enviarTentativaDueto(&dueto, entrada);
                        mensagem[0] = '\0';
                    } else if (resultadoValidacao == 0) {
                        snprintf(mensagem, sizeof(mensagem),
                                "palavra não encontrada...");
                    } else {
                        snprintf(mensagem, sizeof(mensagem),
                                "não foi possível consultar o dicionário");
                    }

                    entrada[0] = '\0';
                }
            }

            if (dueto.terminou && IsKeyPressed(KEY_R)) {
                char nova1[DUETO_TAMANHO + 1];
                char nova2[DUETO_TAMANHO + 1];

                if (escolherPalavra(nova1)) {
                    for (int tentativa = 0; tentativa < 10; tentativa++) {
                        if (escolherPalavra(nova2) &&
                            strcmp(nova1, nova2) != 0) {
                            iniciarDueto(&dueto, nova1, nova2);
                            entrada[0] = '\0';
                            break;
                        }
                    }
                }
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            // ============== DUETO DUTOE ==============
            DrawText("DUETO", 225, 35, 40, DARKGRAY);
            desenharDueto(&dueto, entrada);

            if (dueto.terminou) {
                if (dueto.venceu) {
                    DrawText("Voce acertou as duas palavras!",
                             115, 565, 22, GREEN);
                } else {
                    DrawText("Fim das tentativas!", 180, 550, 24, RED);
                    DrawText(TextFormat("%s / %s",
                             dueto.palavras[0], dueto.palavras[1]),
                             205, 585, 24, DARKGRAY);
                }
                DrawText("Pressione R para jogar novamente", 105, 650, 20, DARKGRAY);

            } else if (mensagem[0] != '\0') {
                DrawText(mensagem, 90, 610, 18, RED);
            } else {
                DrawText("Digite 5 letras e pressione enter", 95, 610, 20, DARKGRAY);
            }

            EndDrawing();
            continue;
        }

        if (!jogo.terminou) {
            int tecla = GetCharPressed();

            while (tecla > 0) {
                if (tecla >= 'a' && tecla <= 'z') {
                    tecla = toupper(tecla);
                }

                if (tecla >= 'A' && tecla <= 'Z' &&
                    strlen(entrada) < TAMANHO) {
                    
                    mensagem[0] = '\0'; // msg do jogo desaparece qnd jogador comeca a digitar
                    int tamanho = (int)strlen(entrada);
                    entrada[tamanho] = (char)tecla;
                    entrada[tamanho + 1] = '\0';
                }

                tecla = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                int tamanho = (int)strlen(entrada);

                if (tamanho > 0) {
                    entrada[tamanho - 1] = '\0';
                }
            }

            if (IsKeyPressed(KEY_ENTER) &&
                strlen(entrada) == TAMANHO) {

                int resultadoValidacao = validar_palavra(entrada);

                if (resultadoValidacao == 1) {
                    enviarTentativa(&jogo, entrada);
                    mensagem[0] = '\0';
                } else if (resultadoValidacao == 0) {
                    snprintf(mensagem, sizeof(mensagem),
                            "palavra não encontrada...");
                } else {
                    snprintf(mensagem, sizeof(mensagem),
                            "não foi possível consultar o dicionário");
                }

                entrada[0] = '\0';
            }

        } else if (!registroSalvo) {
            adicionarResultado(&ranking, "Jogador",
                               jogo.quantidadeTentativas, jogo.venceu);

            salvarResultado("ranking.txt", "Jogador",
                            jogo.quantidadeTentativas, jogo.venceu);

            registroSalvo = 1;
        }

        if (jogo.terminou && IsKeyPressed(KEY_R)) {
            if (escolherPalavra(palavra)) {
                iniciarJogo(&jogo, palavra);
                entrada[0] = '\0';
                registroSalvo = 0;
            } else {
                printf("Erro ao obter nova palavra. Tente novamente.\n");
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);


        // =================== TERMO TERMO TERMO =============== 
        DrawText("TERMO", 220, 40, 40, DARKGRAY);
        desenharJogo(&jogo, entrada);

        if (jogo.terminou) {
            if (jogo.venceu) {
                DrawText("Voce acertou!", 200, 610, 25, GREEN);
            } else {
                DrawText(TextFormat("Palavra: %s", jogo.palavra),
                         200, 610, 25, RED);
            }
            DrawText("Pressione R para jogar novamente", 105, 650, 20, DARKGRAY);
        
        } else if (mensagem[0] != '\0') {
            DrawText(mensagem, 90, 610, 18, RED);
        } else {
            DrawText("Digite 5 letras e pressione enter", 95, 610, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    finalizarAPI();
    liberarRanking(ranking);

    return 0;
}
