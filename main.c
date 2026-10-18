
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "jogo.h"
#include "palavras.h"
#include "ranking.h"

#define LARGURA 600
#define ALTURA 700

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
    srand((unsigned int)time(NULL));

    BancoPalavras banco = {NULL, 0, 0};

    if (!carregarPalavras(&banco, "palavras.txt")) {
        printf("Erro ao carregar palavras.txt\n");
        return 1;
    }

    Resultado *ranking = NULL;
    carregarRanking(&ranking, "ranking.txt");

    Jogo jogo;
    char entrada[TAMANHO + 1] = "";

    char palavra[TAMANHO + 1];
    escolherPalavra(&banco, palavra);
    iniciarJogo(&jogo, palavra);

    int registroSalvo = 0;

    InitWindow(LARGURA, ALTURA, "Termo");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (!jogo.terminou) {
            int tecla = GetCharPressed();

            while (tecla > 0) {
                if (tecla >= 'a' && tecla <= 'z') {
                    tecla = toupper(tecla);
                }

                if (tecla >= 'A' && tecla <= 'Z' &&
                    strlen(entrada) < TAMANHO) {
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
                enviarTentativa(&jogo, entrada);
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
            escolherPalavra(&banco, palavra);
            iniciarJogo(&jogo, palavra);
            entrada[0] = '\0';
            registroSalvo = 0;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("TERMO", 220, 40, 40, DARKGRAY);
        desenharJogo(&jogo, entrada);

        if (jogo.terminou) {
            if (jogo.venceu) {
                DrawText("Voce acertou!", 200, 610, 25, GREEN);
            } else {
                DrawText(TextFormat("Palavra: %s", jogo.palavra),
                         200, 610, 25, RED);
            }

            DrawText("Pressione R para jogar novamente",
                     105, 650, 20, DARKGRAY);
        } else {
            DrawText("Digite 5 letras e pressione ENTER",
                     95, 610, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();

    liberarPalavras(&banco);
    liberarRanking(ranking);

    return 0;
}