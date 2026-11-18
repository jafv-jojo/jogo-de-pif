#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h> 

#include "jogo.h"
#include "palavras.h"
#include "ranking.h"
#include "dueto.h"
#include "quarteto.h"

#define LARGURA 600
#define ALTURA 700

#define TELA_MENU 0
#define TELA_TERMO 1
#define TELA_DUETO 2
#define TELA_QUARTETO 3
#define TELA_SEQUENCIA 4

/*
██████  ███████ ███████ ███████ ███    ██ ██   ██  █████  ██████      ███    ███ ███████ ███    ██ ██    ██ 
██   ██ ██      ██      ██      ████   ██ ██   ██ ██   ██ ██   ██     ████  ████ ██      ████   ██ ██    ██ 
██   ██ █████   ███████ █████   ██ ██  ██ ███████ ███████ ██████      ██ ████ ██ █████   ██ ██  ██ ██    ██ 
██   ██ ██           ██ ██      ██  ██ ██ ██   ██ ██   ██ ██   ██     ██  ██  ██ ██      ██  ██ ██ ██    ██ 
██████  ███████ ███████ ███████ ██   ████ ██   ██ ██   ██ ██   ██     ██      ██ ███████ ██   ████  ██████  
*/
void desenharMenu(void) {

    DrawText("CERMO",
         (600 - MeasureText("CERMO", 40)) / 2,
         90, 40, DARKGRAY);

    DrawText("Escolha o modo de jogo", 155, 155, 22, GRAY);

    DrawRectangle(150, 240, 300, 75, DARKGREEN);
    DrawText("NORMAL",
             150 + (300 - MeasureText("NORMAL", 30)) / 2,
             262, 30, WHITE);

    DrawRectangle(150, 340, 300, 75, GRAY);
    DrawText("DUETO",
             150 + (300 - MeasureText("DUETO", 30)) / 2,
             362, 30, WHITE);

    DrawRectangle(150, 440, 300, 75, GRAY);
    DrawText("QUARTETO",
             150 + (300 - MeasureText("QUARTETO", 30)) / 2,
             462, 30, WHITE);

    DrawRectangle(150, 540, 300, 75, GRAY);
    DrawText("PARTIDAS",
             150 + (300 - MeasureText("PARTIDAS", 25)) / 2,
             565, 25, WHITE);
}

void desenharSequencia(Resultado *historico) {

    Estatisticas estatisticas;

    calcularEstatisticas(historico, &estatisticas);

    DrawText("MINHA SEQUÊNCIA", 145, 35, 32, DARKGRAY);

    DrawText("Estatisticas das partidas no modo normal",
             105, 78, 17, GRAY);

    DrawRectangle(55, 120, 230, 90, LIGHTGRAY);
    DrawRectangleLines(55, 120, 230, 90, DARKGRAY);

    DrawText(TextFormat("%d", estatisticas.totalPartidas),
             75, 132, 35, DARKGRAY);

    DrawText("PARTIDAS JOGADAS", 75, 177, 16, GRAY);

    DrawRectangle(315, 120, 230, 90, LIGHTGRAY);
    DrawRectangleLines(315, 120, 230, 90, DARKGRAY);

    DrawText(TextFormat("%d", estatisticas.totalVitorias),
             335, 132, 35, GREEN);

    DrawText("VITORIAS", 335, 177, 16, GRAY);

    DrawRectangle(55, 225, 230, 90, LIGHTGRAY);
    DrawRectangleLines(55, 225, 230, 90, DARKGRAY);

    DrawText(TextFormat("%d", estatisticas.sequenciaAtual),
             75, 237, 35, DARKGREEN);

    DrawText("SEQUENCIA ATUAL", 75, 282, 16, GRAY);

    DrawRectangle(315, 225, 230, 90, LIGHTGRAY);
    DrawRectangleLines(315, 225, 230, 90, DARKGRAY);

    DrawText(TextFormat("%d", estatisticas.melhorSequencia),
             335, 237, 35, DARKBLUE);

    DrawText("MELHOR SEQUENCIA", 335, 282, 16, GRAY);

    DrawText("HISTORICO RECENTE", 55, 350, 20, DARKGRAY);

    Resultado *atual = historico;

    int limite = 9;
    int indice = 0;

    if (atual == NULL) {
        DrawText("Nenhuma partida registrada ainda.",
                 55, 395, 18, GRAY);
    }

    while (atual != NULL && indice < limite) {

        int numeroPartida = estatisticas.totalPartidas - indice;

        const char *resultado =
            atual->venceu ? "VITORIA" : "DERROTA";

        Color cor = atual->venceu ? GREEN : RED;

        DrawText(TextFormat("Partida %d", numeroPartida),
                 60, 390 + indice * 29, 17, DARKGRAY);

        DrawText(resultado, 245, 390 + indice * 29, 17, cor);

        DrawText(TextFormat("%d tentativas", atual->tentativas),
                 365, 390 + indice * 29, 17, GRAY);

        atual = atual->proximo;

        indice++;
    }

    DrawText("ESC - Voltar ao menu", 195, 665, 18, DARKGRAY);
}
/*
██████  ███████ ███████ ███████ ███    ██ ██   ██  █████  ██████      ██████  ██    ██ ███████ ████████  ██████  
██   ██ ██      ██      ██      ████   ██ ██   ██ ██   ██ ██   ██     ██   ██ ██    ██ ██         ██    ██    ██ 
██   ██ █████   ███████ █████   ██ ██  ██ ███████ ███████ ██████      ██   ██ ██    ██ █████      ██    ██    ██ 
██   ██ ██           ██ ██      ██  ██ ██ ██   ██ ██   ██ ██   ██     ██   ██ ██    ██ ██         ██    ██    ██ 
██████  ███████ ███████ ███████ ██   ████ ██   ██ ██   ██ ██   ██     ██████   ██████  ███████    ██     ██████  
*/

void desenharDueto(Dueto *dueto, char entrada[], int linhaTremendo, float deslocamentoX) {

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
                
                if (i == linhaTremendo) {
                    x += (int)deslocamentoX;
                }

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

/*
██████  ███████ ███████ ███████ ███    ██ ██   ██  █████  ██████       ██████  ██    ██  █████  ██████  ████████ ███████ ████████  ██████  
██   ██ ██      ██      ██      ████   ██ ██   ██ ██   ██ ██   ██     ██    ██ ██    ██ ██   ██ ██   ██    ██    ██         ██    ██    ██ 
██   ██ █████   ███████ █████   ██ ██  ██ ███████ ███████ ██████      ██    ██ ██    ██ ███████ ██████     ██    █████      ██    ██    ██ 
██   ██ ██           ██ ██      ██  ██ ██ ██   ██ ██   ██ ██   ██     ██ ▄▄ ██ ██    ██ ██   ██ ██   ██    ██    ██         ██    ██    ██ 
██████  ███████ ███████ ███████ ██   ████ ██   ██ ██   ██ ██   ██      ██████   ██████  ██   ██ ██   ██    ██    ███████    ██     ██████  
*/

void desenharQuarteto(Quarteto *quarteto, char entrada[], int linhaTremendo, float deslocamentoX) {

    int tamanhoCelula = 27;
    int espaco = 3;

    for (int tab = 0; tab < QUARTETO_QUANTIDADE_PALAVRAS; tab++) {

        int coluna = tab % 2;
        int linhaTabuleiro = tab / 2;

        int inicioX = 80 + coluna * 290;
        int inicioY = 100 + linhaTabuleiro * 290;

        DrawText(TextFormat("PALAVRA %d", tab + 1),
                 inicioX + 23, inicioY - 16, 18, DARKGRAY);

        for (int i = 0; i < QUARTETO_MAX_TENTATIVAS; i++) {

            for (int j = 0; j < QUARTETO_TAMANHO; j++) {

                int x = inicioX + j * (tamanhoCelula + espaco);
                int y = inicioY + i * (tamanhoCelula + espaco);
                
                if (i == linhaTremendo) {
                    x += (int)deslocamentoX;
                }

                Color cor = LIGHTGRAY;

                int mostrar = i < quarteto->quantidadeTentativas &&
                    (!quarteto->acertou[tab] ||
                     i < quarteto->tentativaAcerto[tab]);

                if (mostrar) {

                    if (quarteto->cores[tab][i][j] == 2) {
                        cor = GREEN;
                    } else if (quarteto->cores[tab][i][j] == 1) {
                        cor = GOLD;
                    } else {
                        cor = GRAY;
                    }
                }

                DrawRectangle(x, y, tamanhoCelula, tamanhoCelula, cor);

                DrawRectangleLines(x, y, tamanhoCelula,
                                   tamanhoCelula, DARKGRAY);

                char letra[2] = {'\0', '\0'};

                if (mostrar) {
                    letra[0] = quarteto->tentativas[i][j];

                } else if (i == quarteto->quantidadeTentativas &&
                           !quarteto->acertou[tab] &&
                           strlen(entrada) > (size_t)j) {

                    letra[0] = entrada[j];
                }

                if (letra[0] != '\0') {
                    DrawText(letra, x + 6, y + 3, 20, WHITE);
                }
            }
        }
    }
}
/*
██████  ███████ ███████ ███████ ███    ██ ██   ██  █████  ██████           ██  ██████   ██████   ██████  
██   ██ ██      ██      ██      ████   ██ ██   ██ ██   ██ ██   ██          ██ ██    ██ ██       ██    ██ 
██   ██ █████   ███████ █████   ██ ██  ██ ███████ ███████ ██████           ██ ██    ██ ██   ███ ██    ██ 
██   ██ ██           ██ ██      ██  ██ ██ ██   ██ ██   ██ ██   ██     ██   ██ ██    ██ ██    ██ ██    ██ 
██████  ███████ ███████ ███████ ██   ████ ██   ██ ██   ██ ██   ██      █████   ██████   ██████   ██████  
*/

void desenharJogo(Jogo *jogo, char entrada[], int linhaTremendo, float deslocamentoX) { 

    int inicioX = 150;
    int inicioY = 150;
    int tamanhoCelula = 55;
    int espaco = 7;

    for (int i = 0; i < MAX_TENTATIVAS; i++) {

        for (int j = 0; j < TAMANHO; j++) {

            int x = inicioX + j * (tamanhoCelula + espaco);
            int y = inicioY + i * (tamanhoCelula + espaco);

            if (i == linhaTremendo) { 
                x += (int)deslocamentoX; 
            } 

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

            DrawRectangleLines(x, y, tamanhoCelula,
                               tamanhoCelula, DARKGRAY);

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
/*
██████  ███████ ███████ ███████ ███    ██ ██   ██  █████  ██████      ██████  ███████ ███████ ██    ██ ██      ████████  █████  ██████   ██████  ███████  
██   ██ ██      ██      ██      ████   ██ ██   ██ ██   ██ ██   ██     ██   ██ ██      ██      ██    ██ ██         ██    ██   ██ ██   ██ ██    ██ ██       
██   ██ █████   ███████ █████   ██ ██  ██ ███████ ███████ ██████      ██████  █████   ███████ ██    ██ ██         ██    ███████ ██   ██ ██    ██ ███████  
██   ██ ██           ██ ██      ██  ██ ██ ██   ██ ██   ██ ██   ██     ██   ██ ██           ██ ██    ██ ██         ██    ██   ██ ██   ██ ██    ██      ██  
██████  ███████ ███████ ███████ ██   ████ ██   ██ ██   ██ ██   ██     ██   ██ ███████ ███████  ██████  ███████    ██    ██   ██ ██████   ██████  ███████  
*/
void desenharResultadoTermo(Jogo *jogo, int deslocamento_resultadoY) {

    DrawRectangle(65, 230 + deslocamento_resultadoY, 470, 230, RAYWHITE);
    DrawRectangleLines(65, 230 + deslocamento_resultadoY, 470, 230, DARKGRAY);

    if (jogo->venceu) {
        DrawText("VOCE VENCEU!", 170, 255 + deslocamento_resultadoY, 30, GREEN);
    } else {
        DrawText("VOCE PERDEU... :'(", 170, 255 + deslocamento_resultadoY, 30, RED);
    }

    DrawText("Palavra correta:", 185, 315 + deslocamento_resultadoY, 22, DARKGRAY);

    int larguraTexto = MeasureText(jogo->palavra, 30);

    DrawText(jogo->palavra, (600 - larguraTexto) / 2,
             350  + deslocamento_resultadoY, 30, DARKGRAY);

    DrawText("R - Jogar novamente", 175, 405 + deslocamento_resultadoY, 20, DARKGRAY);
    DrawText("ESC - Voltar ao menu", 175, 435 + deslocamento_resultadoY, 18, DARKGRAY);
}

void desenharResultadoDueto(Dueto *dueto, int deslocamento_resultadoY) {
    DrawRectangle(65, 230 + deslocamento_resultadoY, 470, 240, RAYWHITE);
    DrawRectangleLines(65, 230 + deslocamento_resultadoY, 470, 240, DARKGRAY);

    if (dueto->venceu) {
        DrawText("VOCE VENCEU!", 170, 255 + deslocamento_resultadoY, 30, GREEN);
    } else {
        DrawText("VOCE PERDEU... :'(", 170, 255 + deslocamento_resultadoY, 30, RED);
    }

    DrawText("Palavras corretas:", 175, 310 + deslocamento_resultadoY, 22, DARKGRAY);

    const char *respostas = TextFormat(
        "%s  /  %s",
        dueto->palavras[0],
        dueto->palavras[1]
    );

    int larguraTexto = MeasureText(respostas, 28);

    DrawText(respostas, (600 - larguraTexto) / 2,
             350 + deslocamento_resultadoY, 28, DARKGRAY);

    DrawText("R - Jogar novamente", 175, 405 + deslocamento_resultadoY, 20, DARKGRAY);
    DrawText("ESC - Voltar ao menu", 175, 440 + deslocamento_resultadoY, 18, DARKGRAY);
}

void desenharResultadoQuarteto(Quarteto *quarteto, int deslocamento_resultadoY) {

    DrawRectangle(55, 220 + deslocamento_resultadoY, 490, 270, RAYWHITE);
    DrawRectangleLines(55, 220 + deslocamento_resultadoY, 490, 270, DARKGRAY);

    if (quarteto->venceu) {
        DrawText("VOCE VENCEU!", 170, 245 + deslocamento_resultadoY, 30, GREEN);
    } else {
        DrawText("VOCE PERDEU... :'(", 170, 245 + deslocamento_resultadoY, 30, RED);
    }

    DrawText("Palavras corretas:", 175, 300 + deslocamento_resultadoY, 22, DARKGRAY);

    const char *linha1 = TextFormat(
        "%s / %s",
        quarteto->palavras[0],
        quarteto->palavras[1]
    );

    DrawText(linha1, (600 - MeasureText(linha1, 25)) / 2,
             340 + deslocamento_resultadoY, 25, DARKGRAY);

    const char *linha2 = TextFormat(
        "%s / %s",
        quarteto->palavras[2],
        quarteto->palavras[3]
    );

    DrawText(linha2, (600 - MeasureText(linha2, 25)) / 2,
             375 + deslocamento_resultadoY, 25, DARKGRAY);

    DrawText("R - Jogar novamente", 175, 430 + deslocamento_resultadoY, 20, DARKGRAY);
    DrawText("ESC - Voltar ao menu", 175, 460 + deslocamento_resultadoY, 18, DARKGRAY);
}
/*
███    ███    █████    ██   ███    ██ 
████  ████   ██   ██   ██   ████   ██ 
██ ████ ██   ██   ██   ██   ██ ██  ██ 
██  ██  ██   ███████   ██   ██  ██ ██ 
██      ██   ██   ██   ██   ██   ████
██      ██   ██   ██   ██   ██     ██
*/

int main(void) {
    // variaveis animação de tremer a linha qnd a resposta é inválida
    const float duracaoTremida = 0.50f; 
    double inicioTremor = -1.0; 
    int linhaTremendo = -1; 

    // variaveis da animação da mensagem de resultado
    const float duracaoAnimResultado = 1.2f;
    const float distanciaAnimResultado = 480.0f;
    double inicioAnimResultado = -1.0;



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

    Quarteto quarteto;

    char palavrasQuarteto[QUARTETO_QUANTIDADE_PALAVRAS]
                         [QUARTETO_TAMANHO + 1];

    int quantidadePalavras = 0;
    int tentativasBusca = 0;

    while (quantidadePalavras < QUARTETO_QUANTIDADE_PALAVRAS &&
           tentativasBusca < 40) {

        char candidata[QUARTETO_TAMANHO + 1];

        tentativasBusca++;

        if (!escolherPalavra(candidata)) {
            continue;
        }

        int repetida = 0;

        for (int i = 0; i < quantidadePalavras; i++) {

            if (strcmp(candidata, palavrasQuarteto[i]) == 0) {
                repetida = 1;
                break;
            }
        }

        if (!repetida) {
            strcpy(palavrasQuarteto[quantidadePalavras], candidata);
            quantidadePalavras++;
        }
    }

    if (quantidadePalavras != QUARTETO_QUANTIDADE_PALAVRAS) {

        printf("Erro ao obter quatro palavras diferentes.\n");

        finalizarAPI();
        liberarRanking(ranking);

        return 1;
    }

    iniciarQuarteto(&quarteto,
                    palavrasQuarteto[0],
                    palavrasQuarteto[1],
                    palavrasQuarteto[2],
                    palavrasQuarteto[3]);

    int registroSalvo = 0;

    InitWindow(LARGURA, ALTURA, "Cermo");

    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deslocamentoX = 0.0f; // 584 - 603: calculos pra animacao de tremer linha

        if (inicioTremor >= 0.0) {
            float tempoDecorrido = (float)(GetTime() - inicioTremor);

            if (tempoDecorrido < duracaoTremida) {
                float intensidade = 1.0f - (tempoDecorrido / duracaoTremida);
                deslocamentoX =
                    sinf(tempoDecorrido * 70.0f) * 20.0f * intensidade;
            } else {
                inicioTremor = -1.0;
                linhaTremendo = -1;
            }
        }

        int resultado_aparecendo =
            (telaAtual == TELA_TERMO && jogo.terminou) ||
            (telaAtual == TELA_DUETO && dueto.terminou) ||
            (telaAtual == TELA_QUARTETO && quarteto.terminou);

        if (resultado_aparecendo && inicioAnimResultado < 0.0) {
            inicioAnimResultado = GetTime();
        }

        float deslocamento_resultado = 0.0f;

        if (resultado_aparecendo && inicioAnimResultado >= 0.0) {
            float tempo_passado = (float)(GetTime() - inicioAnimResultado);

            if (tempo_passado < duracaoAnimResultado) {
                float restante = 1.0f - (tempo_passado / duracaoAnimResultado);
                deslocamento_resultado = distanciaAnimResultado * restante * restante;
            }
        }

        int deslocamento_resultadoY = (int)deslocamento_resultado;

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

                if (CheckCollisionPointRec(
                    mouse, (Rectangle){150, 440, 300, 75})) {

                    telaAtual = TELA_QUARTETO;
                    entrada[0] = '\0';
                    mensagem[0] = '\0';
                }

                if (CheckCollisionPointRec(mouse, (Rectangle){150, 540, 300, 75})) {
                    telaAtual = TELA_SEQUENCIA;
                }
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharMenu();

            EndDrawing();

            continue;
        }

        if (telaAtual == TELA_SEQUENCIA) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                telaAtual = TELA_MENU;
                continue;
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharSequencia(ranking);

            EndDrawing();

            continue;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {

            telaAtual = TELA_MENU;
            entrada[0] = '\0';

            continue;
        }

        if (telaAtual == TELA_QUARTETO) {

            if (quarteto.terminou && IsKeyPressed(KEY_R)) {

                char novasPalavras[QUARTETO_QUANTIDADE_PALAVRAS]
                                  [QUARTETO_TAMANHO + 1];

                int quantidade = 0;
                int tentativas = 0;

                while (quantidade < QUARTETO_QUANTIDADE_PALAVRAS &&
                       tentativas < 40) {

                    char candidata[QUARTETO_TAMANHO + 1];

                    tentativas++;

                    if (!escolherPalavra(candidata)) {
                        continue;
                    }

                    int repetida = 0;

                    for (int i = 0; i < quantidade; i++) {

                        if (strcmp(candidata, novasPalavras[i]) == 0) {
                            repetida = 1;
                            break;
                        }
                    }

                    if (!repetida) {
                        strcpy(novasPalavras[quantidade], candidata);
                        quantidade++;
                    }
                }

                if (quantidade == QUARTETO_QUANTIDADE_PALAVRAS) {

                    iniciarQuarteto(&quarteto,
                                   novasPalavras[0],
                                   novasPalavras[1],
                                   novasPalavras[2],
                                   novasPalavras[3]);
                    
                    inicioAnimResultado = -1.0;
                    entrada[0] = '\0';
                    mensagem[0] = '\0';

                } else {
                    printf("Erro ao obter novas palavras do Quarteto.\n");
                }
            }

            if (!quarteto.terminou) {

                int tecla = GetCharPressed();

                while (tecla > 0) {

                    if (tecla >= 'a' && tecla <= 'z') {
                        tecla = toupper(tecla);
                    }

                    if (tecla >= 'A' && tecla <= 'Z' &&
                        strlen(entrada) < QUARTETO_TAMANHO) {

                        mensagem[0] = '\0';

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
                    strlen(entrada) == QUARTETO_TAMANHO) {

                    int resultado = validar_palavra(entrada);
                    
                    if (resultado != 1) { //animacao de quando a tentativa é inválida ou quando dá erro na consulta do dicionário
                        linhaTremendo = quarteto.quantidadeTentativas;
                        inicioTremor = GetTime();
                    }

                    if (resultado == 1) {

                        enviarTentativaQuarteto(&quarteto, entrada);
                        mensagem[0] = '\0';

                    } else if (resultado == 0) {

                        snprintf(mensagem, sizeof(mensagem),
                                 "Palavra nao encontrada");

                    } else {

                        snprintf(mensagem, sizeof(mensagem),
                                 "Erro ao consultar o dicionario");
                    }

                    entrada[0] = '\0';
                }
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("QUARTETO", 185, 25, 35, DARKGRAY);

            desenharQuarteto(&quarteto, entrada, linhaTremendo, deslocamentoX);

            if (quarteto.terminou) {

                desenharResultadoQuarteto(&quarteto, deslocamento_resultadoY);

            } else if (mensagem[0] != '\0') {

                DrawText(mensagem, 115, 665, 18, RED);
            }

            EndDrawing();

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

                        mensagem[0] = '\0';

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
                    
                    if (resultadoValidacao != 1) { //animacao de quando a tentativa é inválida ou quando dá erro na consulta do dicionário
                        linhaTremendo = dueto.quantidadeTentativas;
                        inicioTremor = GetTime();
                    }

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
                            inicioAnimResultado = -1.0;
                            entrada[0] = '\0';

                            break;
                        }
                    }
                }
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("DUETO", 225, 35, 40, DARKGRAY);

            desenharDueto(&dueto, entrada, linhaTremendo, deslocamentoX);

            if (dueto.terminou) {

                desenharResultadoDueto(&dueto, deslocamento_resultadoY);

            } else if (mensagem[0] != '\0') {

                DrawText(mensagem, 90, 610, 18, RED);

            } else {

                DrawText("Digite 5 letras e pressione enter",
                         95, 610, 20, DARKGRAY);
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

                    mensagem[0] = '\0';

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

                if (resultadoValidacao != 1) { //animacao de quando a tentativa é inválida ou quando dá erro na consulta do dicionário
                    linhaTremendo = jogo.quantidadeTentativas;
                    inicioTremor = GetTime();
                }

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

                // variaveis para animacoes:
                inicioAnimResultado = -1.0;
                inicioTremor = -1.0;
                linhaTremendo = -1;

                //

                registroSalvo = 0;

            } else {

                printf("Erro ao obter nova palavra. Tente novamente.\n");
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("CERMO", 220, 40, 40, DARKGRAY);

        desenharJogo(&jogo, entrada, linhaTremendo, deslocamentoX); 

        if (jogo.terminou) {

            desenharResultadoTermo(&jogo, deslocamento_resultadoY);

        } else if (mensagem[0] != '\0') {

            DrawText(mensagem, 90, 610, 18, RED);

        } else {

            DrawText("Digite 5 letras e pressione enter",
                     95, 610, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();

    finalizarAPI();

    liberarRanking(ranking);

    return 0;
}
