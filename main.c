#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "jogo.h"
#include "ranking.h"
#include "dueto.h"
#include "quarteto.h"

#define LARGURA 600
#define ALTURA 800

#define TELA_MENU 0
#define TELA_TERMO 1
#define TELA_DUETO 2
#define TELA_QUARTETO 3
#define TELA_SEQUENCIA 4

typedef struct {
    const char *utf8;
    char letra;
} AcentoTxt;

typedef struct {
    char **palavras;
    int total;
} ListaPalavras;

ListaPalavras listaPalavras = {NULL, 0};

int converterLinhaTxt(const char *linha, char destino[TAMANHO + 1]) {

    AcentoTxt acentos[] = {
        {"á", 'A'}, {"à", 'A'}, {"â", 'A'}, {"ã", 'A'},
        {"Á", 'A'}, {"À", 'A'}, {"Â", 'A'}, {"Ã", 'A'},
        {"é", 'E'}, {"è", 'E'}, {"ê", 'E'},
        {"É", 'E'}, {"È", 'E'}, {"Ê", 'E'},
        {"í", 'I'}, {"ì", 'I'}, {"î", 'I'},
        {"Í", 'I'}, {"Ì", 'I'}, {"Î", 'I'},
        {"ó", 'O'}, {"ò", 'O'}, {"ô", 'O'}, {"õ", 'O'},
        {"Ó", 'O'}, {"Ò", 'O'}, {"Ô", 'O'}, {"Õ", 'O'},
        {"ú", 'U'}, {"ù", 'U'}, {"û", 'U'}, {"ü", 'U'},
        {"Ú", 'U'}, {"Ù", 'U'}, {"Û", 'U'}, {"Ü", 'U'},
        {"ç", 'C'}, {"Ç", 'C'}
    };

    int totalAcentos = (int)(sizeof(acentos) / sizeof(acentos[0]));
    int posicao = 0;

    while (*linha != '\0' && *linha != '\r' && *linha != '\n') {

        unsigned char atual = (unsigned char)*linha;
        char letra = '\0';
        int bytes = 1;

        if (atual >= 'a' && atual <= 'z') {
            letra = (char)(atual - 'a' + 'A');
        } else if (atual >= 'A' && atual <= 'Z') {
            letra = (char)atual;
        } else {
            for (int i = 0; i < totalAcentos; i++) {
                int tamanhoAcento = (int)strlen(acentos[i].utf8);

                if (strncmp(linha, acentos[i].utf8, tamanhoAcento) == 0) {
                    letra = acentos[i].letra;
                    bytes = tamanhoAcento;
                    break;
                }
            }

            if (letra == '\0') {
                return 0;
            }
        }

        if (posicao >= TAMANHO) {
            return 0;
        }

        destino[posicao] = letra;
        posicao++;
        linha += bytes;
    }

    if (posicao != TAMANHO) {
        return 0;
    }

    destino[posicao] = '\0';

    return 1;
}

int carregarPalavrasTxt(const char arquivo[], ListaPalavras *lista) {

    FILE *f = fopen(arquivo, "r");

    if (f == NULL) {
        return 0;
    }

    char linha[64];

    while (fgets(linha, sizeof(linha), f) != NULL) {

        char palavra[TAMANHO + 1];

        if (!converterLinhaTxt(linha, palavra)) {
            continue;
        }

        char **novaLista = realloc(lista->palavras, (lista->total + 1) * sizeof(char *));

        if (novaLista == NULL) {
            break;
        }

        lista->palavras = novaLista;
        lista->palavras[lista->total] = malloc(TAMANHO + 1);

        if (lista->palavras[lista->total] == NULL) {
            break;
        }

        strcpy(lista->palavras[lista->total], palavra);
        lista->total++;
    }

    fclose(f);

    return lista->total;
}

int sortearPalavraTxt(char destino[]) {

    if (listaPalavras.total == 0) {
        return 0;
    }

    strcpy(destino, listaPalavras.palavras[rand() % listaPalavras.total]);

    return 1;
}

int estaNaLista(ListaPalavras *lista, const char palavra[]) {

    for (int i = 0; i < lista->total; i++) {
        if (strcmp(lista->palavras[i], palavra) == 0) {
            return 1;
        }
    }

    return 0;
}

int validarPalavraTxt(const char tentativa[]) {

    if (estaNaLista(&listaPalavras, tentativa)) {
        return 1;
    }

    return 0;
}

void liberarLista(ListaPalavras *lista) {

    for (int i = 0; i < lista->total; i++) {
        free(lista->palavras[i]);
    }

    free(lista->palavras);

    lista->palavras = NULL;
    lista->total = 0;
}




#define TEMPO_LETRA 0.15
#define TOTAL_CONFETES 150

double inicioRevelacao = -10.0;

int revelacaoAcabou(void) {
    return GetTime() - inicioRevelacao > 5 * TEMPO_LETRA + 0.2;
}

typedef struct {
    float x;
    float y;
    float velocidadeX;
    float velocidadeY;
    Color cor;
} Confete;

Confete confetes[TOTAL_CONFETES];
int confeteAtivo = 0;

void iniciarConfete(void) {

    Color cores[4] = {GREEN, GOLD, BLUE, RED};

    for (int i = 0; i < TOTAL_CONFETES; i++) {
        confetes[i].x = GetRandomValue(0, LARGURA);
        confetes[i].y = GetRandomValue(-500, 0);
        confetes[i].velocidadeX = GetRandomValue(-60, 60);
        confetes[i].velocidadeY = GetRandomValue(150, 350);
        confetes[i].cor = cores[GetRandomValue(0, 3)];
    }

    confeteAtivo = 1;
}

void atualizarConfete(void) {

    if (!confeteAtivo) {
        return;
    }

    float dt = GetFrameTime();

    for (int i = 0; i < TOTAL_CONFETES; i++) {

        confetes[i].x += confetes[i].velocidadeX * dt;
        confetes[i].y += confetes[i].velocidadeY * dt;

        if (confetes[i].y > ALTURA - 8) {
            confetes[i].y = ALTURA - 8;
            confetes[i].velocidadeX = 0;
            confetes[i].velocidadeY = 0;
        }
    }
}

void desenharConfete(void) {

    if (!confeteAtivo) {
        return;
    }

    for (int i = 0; i < TOTAL_CONFETES; i++) {
        DrawRectangle((int)confetes[i].x, (int)confetes[i].y, 8, 8, confetes[i].cor);
    }
}

void textoCentralizado(const char *texto, int y, int tamanho, Color cor) {

    int larguraTexto = MeasureText(texto, tamanho);

    DrawText(texto, (LARGURA - larguraTexto) / 2, y, tamanho, cor);
}

void desenharBotao(Rectangle botao, const char *texto, Color cor) {

    if (CheckCollisionPointRec(GetMousePosition(), botao)) {
        botao.x -= 5;
        botao.y -= 5;
        botao.width += 10;
        botao.height += 10;
        cor = ColorBrightness(cor, 0.2f);
    }

    Rectangle sombra = {botao.x, botao.y + 6, botao.width, botao.height};

    DrawRectangleRounded(sombra, 0.3f, 10, Fade(BLACK, 0.2f));
    DrawRectangleRounded(botao, 0.3f, 10, cor);

    int larguraTexto = MeasureText(texto, 26);

    DrawText(texto,
             (int)(botao.x + (botao.width - larguraTexto) / 2),
             (int)(botao.y + (botao.height - 26) / 2),
             26, WHITE);
}

RenderTexture2D tela;

void comecarDesenho(void) {
    BeginTextureMode(tela);
}

void terminarDesenho(void) {

    EndTextureMode();

    float escalaX = (float)GetScreenWidth() / LARGURA;
    float escalaY = (float)GetScreenHeight() / ALTURA;
    float escala = escalaX;

    if (escalaY < escalaX) {
        escala = escalaY;
    }

    float larguraFinal = LARGURA * escala;
    float alturaFinal = ALTURA * escala;

    float sobraX = (GetScreenWidth() - larguraFinal) / 2;
    float sobraY = (GetScreenHeight() - alturaFinal) / 2;

    BeginDrawing();

    ClearBackground(BLACK);

    DrawTexturePro(tela.texture,
                   (Rectangle){0, 0, LARGURA, -ALTURA},
                   (Rectangle){sobraX, sobraY, larguraFinal, alturaFinal},
                   (Vector2){0, 0}, 0, WHITE);

    EndDrawing();

    SetMouseOffset((int)-sobraX, (int)-sobraY);
    SetMouseScale(1 / escala, 1 / escala);
}

const char *linha1 = "QWERTYUIOP";
const char *linha2 = "ASDFGHJKL";
const char *linha3 = "ZXCVBNM";

void limparTeclado(int teclado[26]) {

    for (int i = 0; i < 26; i++) {
        teclado[i] = -1;
    }
}

void marcarTeclado(int teclado[26], char tentativa[], int cores[]) {

    for (int j = 0; j < TAMANHO; j++) {

        int posicao = tentativa[j] - 'A';

        if (cores[j] > teclado[posicao]) {
            teclado[posicao] = cores[j];
        }
    }
}

void desenharLetra(char letra, int x, int y, int teclado[26]) {

    int estado = teclado[letra - 'A'];

    Color cor = LIGHTGRAY;
    Color corTexto = DARKGRAY;

    if (estado == VERDE) {
        cor = GREEN;
        corTexto = WHITE;
    } else if (estado == AMARELO) {
        cor = GOLD;
        corTexto = WHITE;
    } else if (estado == CINZA) {
        cor = DARKGRAY;
        corTexto = WHITE;
    }

    char texto[2] = {letra, '\0'};

    Rectangle tecla = {x, y, 48, 34};

    if (CheckCollisionPointRec(GetMousePosition(), tecla)) {
        cor = ColorBrightness(cor, -0.15f);
    }

    DrawRectangleRounded(tecla, 0.3f, 6, cor);
    DrawText(texto, x + (48 - MeasureText(texto, 20)) / 2, y + 7, 20, corTexto);
}

void desenharTeclado(int teclado[26]) {

    for (int i = 0; i < 10; i++) {
        desenharLetra(linha1[i], 33 + i * 54, 685, teclado);
    }

    for (int i = 0; i < 9; i++) {
        desenharLetra(linha2[i], 60 + i * 54, 724, teclado);
    }

    DrawRectangleRounded((Rectangle){33, 763, 75, 34}, 0.3f, 6, LIGHTGRAY);
    DrawText("ENTER", 33 + (75 - MeasureText("ENTER", 16)) / 2, 772, 16, DARKGRAY);

    for (int i = 0; i < 7; i++) {
        desenharLetra(linha3[i], 114 + i * 54, 763, teclado);
    }

    DrawRectangleRounded((Rectangle){492, 763, 75, 34}, 0.3f, 6, LIGHTGRAY);
    DrawText("APAGAR", 492 + (75 - MeasureText("APAGAR", 16)) / 2, 772, 16, DARKGRAY);
}

char teclaClicada(void) {

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return 0;
    }

    Vector2 mouse = GetMousePosition();

    for (int i = 0; i < 10; i++) {
        if (CheckCollisionPointRec(mouse, (Rectangle){33 + i * 54, 685, 48, 34})) {
            return linha1[i];
        }
    }

    for (int i = 0; i < 9; i++) {
        if (CheckCollisionPointRec(mouse, (Rectangle){60 + i * 54, 724, 48, 34})) {
            return linha2[i];
        }
    }

    for (int i = 0; i < 7; i++) {
        if (CheckCollisionPointRec(mouse, (Rectangle){114 + i * 54, 763, 48, 34})) {
            return linha3[i];
        }
    }

    if (CheckCollisionPointRec(mouse, (Rectangle){33, 763, 75, 34})) {
        return '\n';
    }

    if (CheckCollisionPointRec(mouse, (Rectangle){492, 763, 75, 34})) {
        return '\b';
    }

    return 0;
}

/*
██████  ███████ ███████ ███████ ███    ██ ██   ██  █████  ██████      ███    ███ ███████ ███    ██ ██    ██ 
██   ██ ██      ██      ██      ████   ██ ██   ██ ██   ██ ██   ██     ████  ████ ██      ████   ██ ██    ██ 
██   ██ █████   ███████ █████   ██ ██  ██ ███████ ███████ ██████      ██ ████ ██ █████   ██ ██  ██ ██    ██ 
██   ██ ██           ██ ██      ██  ██ ██ ██   ██ ██   ██ ██   ██     ██  ██  ██ ██      ██  ██ ██ ██    ██ 
██████  ███████ ███████ ███████ ██   ████ ██   ██ ██   ██ ██   ██     ██      ██ ███████ ██   ████  ██████  
*/
void desenharMenu(void) {

    int xTitulo = (600 - MeasureText("CERMO", 40)) / 2;
    int passo = (int)(GetTime() * 8) % 12;
    int yTitulo = 85 + passo;

    if (passo > 6) {
        yTitulo = 85 + (12 - passo);
    }

    DrawText("C", xTitulo, yTitulo, 40, BLUE);
    DrawText("ERMO", xTitulo + MeasureText("C", 40) + 4, yTitulo, 40, DARKGRAY);

    textoCentralizado("Escolha o modo de jogo", 155, 22, GRAY);

    desenharBotao((Rectangle){150, 240, 300, 75}, "NORMAL", (Color){106, 170, 100, 255});
    desenharBotao((Rectangle){150, 340, 300, 75}, "DUETO", (Color){201, 180, 88, 255});
    desenharBotao((Rectangle){150, 440, 300, 75}, "QUARTETO", (Color){80, 140, 220, 255});
    desenharBotao((Rectangle){150, 540, 300, 75}, "PARTIDAS", GRAY);

    if (IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE)) {
        desenharBotao((Rectangle){150, 640, 300, 75}, "SAIR DA TELA CHEIA", DARKGRAY);
    } else {
        desenharBotao((Rectangle){150, 640, 300, 75}, "TELA CHEIA", DARKGRAY);
    }

    textoCentralizado("F11 - tela cheia", 740, 20, GRAY);
}

double inicioRanking = 0;

void desenharCartao(int x, int y, int valor, const char *nome, Color cor) {

    DrawRectangleRounded((Rectangle){x, y + 4, 245, 80}, 0.25f, 10, Fade(BLACK, 0.1f));
    DrawRectangleRounded((Rectangle){x, y, 245, 80}, 0.25f, 10, WHITE);

    const char *numero = TextFormat("%d", valor);

    DrawText(numero, x + (245 - MeasureText(numero, 36)) / 2, y + 10, 36, cor);
    DrawText(nome, x + (245 - MeasureText(nome, 15)) / 2, y + 54, 15, GRAY);
}

void desenharSequencia(Resultado *historico) {

    Estatisticas estatisticas;

    calcularEstatisticas(historico, &estatisticas);

    textoCentralizado("MINHA SEQUENCIA", 30, 34, DARKGRAY);

    int porcentagem = 0;

    if (estatisticas.totalPartidas > 0) {
        porcentagem = estatisticas.totalVitorias * 100 / estatisticas.totalPartidas;
    }

    const char *subtitulo = TextFormat("Modo normal - %d%% de vitorias", porcentagem);

    textoCentralizado(subtitulo, 72, 18, GRAY);

    desenharCartao(45, 110, estatisticas.totalPartidas, "PARTIDAS JOGADAS", DARKGRAY);
    desenharCartao(310, 110, estatisticas.totalVitorias, "VITORIAS", (Color){106, 170, 100, 255});
    desenharCartao(45, 205, estatisticas.sequenciaAtual, "SEQUENCIA ATUAL", (Color){201, 160, 40, 255});
    desenharCartao(310, 205, estatisticas.melhorSequencia, "MELHOR SEQUENCIA", (Color){80, 140, 220, 255});

    DrawText("DISTRIBUICAO DE TENTATIVAS", 45, 310, 20, DARKGRAY);

    int distribuicao[6] = {0, 0, 0, 0, 0, 0};

    Resultado *atual = historico;

    while (atual != NULL) {

        if (atual->venceu && atual->tentativas >= 1 && atual->tentativas <= 6) {
            distribuicao[atual->tentativas - 1]++;
        }

        atual = atual->proximo;
    }

    int maior = 1;

    for (int i = 0; i < 6; i++) {
        if (distribuicao[i] > maior) {
            maior = distribuicao[i];
        }
    }

    float crescimento = (GetTime() - inicioRanking) * 2;

    if (crescimento > 1) {
        crescimento = 1;
    }

    for (int i = 0; i < 6; i++) {

        int y = 345 + i * 30;
        int largura = 35 + distribuicao[i] * 420 / maior * crescimento;

        DrawText(TextFormat("%d", i + 1), 50, y + 3, 20, DARKGRAY);

        DrawRectangleRounded((Rectangle){75, y, largura, 24}, 0.4f, 6, (Color){106, 170, 100, 255});

        DrawText(TextFormat("%d", distribuicao[i]), 75 + largura - 22, y + 3, 18, WHITE);
    }

    DrawText("ULTIMAS PARTIDAS", 45, 535, 20, DARKGRAY);

    atual = historico;

    int indice = 0;

    if (atual == NULL) {
        DrawText("Nenhuma partida registrada ainda.", 45, 575, 18, GRAY);
    }

    while (atual != NULL && indice < 4) {

        int y = 568 + indice * 38;

        DrawRectangleRounded((Rectangle){45, y, 510, 32}, 0.4f, 8, WHITE);

        DrawText(TextFormat("Partida %d", estatisticas.totalPartidas - indice), 60, y + 8, 18, DARKGRAY);

        if (atual->venceu) {
            DrawRectangleRounded((Rectangle){235, y + 4, 110, 24}, 0.5f, 8, (Color){106, 170, 100, 255});
            DrawText("VITORIA", 235 + (110 - MeasureText("VITORIA", 16)) / 2, y + 8, 16, WHITE);
        } else {
            DrawRectangleRounded((Rectangle){235, y + 4, 110, 24}, 0.5f, 8, (Color){220, 80, 80, 255});
            DrawText("DERROTA", 235 + (110 - MeasureText("DERROTA", 16)) / 2, y + 8, 16, WHITE);
        }

        DrawText(TextFormat("%d tentativas", atual->tentativas), 405, y + 8, 18, GRAY);

        atual = atual->proximo;

        indice++;
    }

    desenharBotao((Rectangle){200, 730, 200, 50}, "VOLTAR", DARKGRAY);
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

        const char *titulo = TextFormat("PALAVRA %d", tab + 1);
        int larguraTabuleiro = 5 * tamanhoCelula + 4 * espaco;

        DrawText(titulo, inicioX + (larguraTabuleiro - MeasureText(titulo, 20)) / 2, 100, 20, DARKGRAY);

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

                if (i == dueto->quantidadeTentativas - 1) {

                    float tempo = GetTime() - inicioRevelacao;

                    if (tempo < j * TEMPO_LETRA) {
                        cor = LIGHTGRAY;
                    } else if (tempo < j * TEMPO_LETRA + TEMPO_LETRA) {
                        y -= 6;
                    }
                }

                DrawRectangleRounded((Rectangle){x, y, tamanhoCelula, tamanhoCelula}, 0.15f, 6, cor);

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
            DrawText("ACERTOU!", inicioX + (larguraTabuleiro - MeasureText("ACERTOU!", 20)) / 2, 510, 20, GREEN);
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

        const char *titulo = TextFormat("PALAVRA %d", tab + 1);
        int larguraTabuleiro = 5 * tamanhoCelula + 4 * espaco;

        DrawText(titulo, inicioX + (larguraTabuleiro - MeasureText(titulo, 18)) / 2, inicioY - 16, 18, DARKGRAY);

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

                if (i == quarteto->quantidadeTentativas - 1) {

                    float tempo = GetTime() - inicioRevelacao;

                    if (tempo < j * TEMPO_LETRA) {
                        cor = LIGHTGRAY;
                    } else if (tempo < j * TEMPO_LETRA + TEMPO_LETRA) {
                        y -= 6;
                    }
                }

                DrawRectangleRounded((Rectangle){x, y, tamanhoCelula, tamanhoCelula}, 0.15f, 6, cor);

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

void desenharJogo(Jogo *jogo, char entrada[], int linhaTremendo,
                  float deslocamentoX, int cursorTermo) {

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

            if (i == jogo->quantidadeTentativas - 1) {

                float tempo = GetTime() - inicioRevelacao;

                if (tempo < j * TEMPO_LETRA) {
                    cor = LIGHTGRAY;
                } else if (tempo < j * TEMPO_LETRA + TEMPO_LETRA) {
                    y -= 6;
                }
            }

            DrawRectangleRounded((Rectangle){x, y, tamanhoCelula, tamanhoCelula}, 0.15f, 6, cor);
            if (i == jogo->quantidadeTentativas &&
                j == cursorTermo &&
                !jogo->terminou) {

                DrawRectangleLinesEx(
                    (Rectangle){x, y, tamanhoCelula, tamanhoCelula},
                    3,
                    BLUE
                );
            }

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
        textoCentralizado("VOCE VENCEU!", 255 + deslocamento_resultadoY, 30, GREEN);
    } else {
        textoCentralizado("VOCE PERDEU... :'(", 255 + deslocamento_resultadoY, 30, RED);
    }

    textoCentralizado("Palavra correta:", 315 + deslocamento_resultadoY, 22, DARKGRAY);

    textoCentralizado(jogo->palavra, 350 + deslocamento_resultadoY, 30, DARKGRAY);

    textoCentralizado("R - Jogar novamente", 405 + deslocamento_resultadoY, 20, DARKGRAY);
    textoCentralizado("ESC - Voltar ao menu", 435 + deslocamento_resultadoY, 18, DARKGRAY);
}

void desenharResultadoDueto(Dueto *dueto, int deslocamento_resultadoY) {
    DrawRectangle(65, 230 + deslocamento_resultadoY, 470, 240, RAYWHITE);
    DrawRectangleLines(65, 230 + deslocamento_resultadoY, 470, 240, DARKGRAY);

    if (dueto->venceu) {
        textoCentralizado("VOCE VENCEU!", 255 + deslocamento_resultadoY, 30, GREEN);
    } else {
        textoCentralizado("VOCE PERDEU... :'(", 255 + deslocamento_resultadoY, 30, RED);
    }

    textoCentralizado("Palavras corretas:", 310 + deslocamento_resultadoY, 22, DARKGRAY);

    const char *respostas = TextFormat(
        "%s  /  %s",
        dueto->palavras[0],
        dueto->palavras[1]
    );

    textoCentralizado(respostas, 350 + deslocamento_resultadoY, 28, DARKGRAY);

    textoCentralizado("R - Jogar novamente", 405 + deslocamento_resultadoY, 20, DARKGRAY);
    textoCentralizado("ESC - Voltar ao menu", 440 + deslocamento_resultadoY, 18, DARKGRAY);
}

void desenharResultadoQuarteto(Quarteto *quarteto, int deslocamento_resultadoY) {

    DrawRectangle(55, 220 + deslocamento_resultadoY, 490, 270, RAYWHITE);
    DrawRectangleLines(55, 220 + deslocamento_resultadoY, 490, 270, DARKGRAY);

    if (quarteto->venceu) {
        textoCentralizado("VOCE VENCEU!", 245 + deslocamento_resultadoY, 30, GREEN);
    } else {
        textoCentralizado("VOCE PERDEU... :'(", 245 + deslocamento_resultadoY, 30, RED);
    }

    textoCentralizado("Palavras corretas:", 300 + deslocamento_resultadoY, 22, DARKGRAY);

    const char *linha1 = TextFormat(
        "%s / %s",
        quarteto->palavras[0],
        quarteto->palavras[1]
    );

    textoCentralizado(linha1, 340 + deslocamento_resultadoY, 25, DARKGRAY);

    const char *linha2 = TextFormat(
        "%s / %s",
        quarteto->palavras[2],
        quarteto->palavras[3]
    );

    textoCentralizado(linha2, 375 + deslocamento_resultadoY, 25, DARKGRAY);

    textoCentralizado("R - Jogar novamente", 430 + deslocamento_resultadoY, 20, DARKGRAY);
    textoCentralizado("ESC - Voltar ao menu", 460 + deslocamento_resultadoY, 18, DARKGRAY);
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
    
    srand((unsigned int)time(NULL));

    if (carregarPalavrasTxt("palavras_5_letras.txt", &listaPalavras) == 0) {
        printf("Erro ao carregar palavras_5_letras.txt.\n");
        return 1;
    }

    char palavra[TAMANHO + 1];

    if (!sortearPalavraTxt(palavra)) {
        printf("Erro ao sortear a palavra.\n");
        return 1;
    }

    Resultado *ranking = NULL;

    carregarRanking(&ranking, "ranking.txt");

    Jogo jogo;

    char entrada[TAMANHO + 1] = "";
    int cursorTermo = 0;
    char mensagem[64] = "";

    iniciarJogo(&jogo, palavra);
        for (int i = 0; i < TAMANHO; i++) {
            entrada[i] = ' ';
        }
        entrada[TAMANHO] = '\0';

    Dueto dueto;

    char palavraDueto1[DUETO_TAMANHO + 1];
    char palavraDueto2[DUETO_TAMANHO + 1];

    if (!sortearPalavraTxt(palavraDueto1)) {
        printf("Erro ao obter a primeira palavra do Dueto.\n");
        liberarRanking(ranking);
        return 1;
    }

    int palavraDiferente = 0;

    for (int tentativa = 0; tentativa < 10; tentativa++) {

        if (sortearPalavraTxt(palavraDueto2) &&
            strcmp(palavraDueto1, palavraDueto2) != 0) {

            palavraDiferente = 1;
            break;
        }
    }

    if (!palavraDiferente) {
        printf("Erro ao obter duas palavras diferentes.\n");
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

        if (!sortearPalavraTxt(candidata)) {
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

    Image icone = GenImageColor(64, 64, RAYWHITE);

    int larguraC = MeasureText("C", 60);

    ImageDrawText(&icone, "C", (64 - larguraC) / 2, 5, 60, BLUE);
    SetWindowIcon(icone);
    UnloadImage(icone);

    tela = LoadRenderTexture(LARGURA, ALTURA);
    SetTextureFilter(tela.texture, TEXTURE_FILTER_BILINEAR);
    
    // variaveis sfx
    InitAudioDevice();
    
    Sound buzzer = LoadSound("sons/sfx/buzzer.wav");
    Sound resp_ok = LoadSound("sons/sfx/resp_ok.wav");
    SetSoundVolume(buzzer, 0.5f);
    SetSoundVolume(resp_ok, 0.5f);    

    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_F11)) {
            ToggleBorderlessWindowed();
        }
        float deslocamentoX = 0.0f; // 584 - 603: calculos pra animacao de tremer linha

        if (inicioTremor >= 0.0) {
            float tempoDecorrido = (float)(GetTime() - inicioTremor);

            if (tempoDecorrido < duracaoTremida) {
                float intensidade = 1.0f - (tempoDecorrido / duracaoTremida);
                int lado = (int)(tempoDecorrido * 25) % 2;

                if (lado == 0) {
                    deslocamentoX = 15.0f * intensidade;
                } else {
                    deslocamentoX = -15.0f * intensidade;
                }
            } else {
                inicioTremor = -1.0;
                linhaTremendo = -1;
            }
        }

        int resultado_aparecendo =
            (telaAtual == TELA_TERMO && jogo.terminou) ||
            (telaAtual == TELA_DUETO && dueto.terminou) ||
            (telaAtual == TELA_QUARTETO && quarteto.terminou);

        if (!revelacaoAcabou()) {
            resultado_aparecendo = 0;
        }

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

                for (int i = 0; i < TAMANHO; i++) {
                    entrada[i] = ' ';
                }

                entrada[TAMANHO] = '\0';
                cursorTermo = 0;
                mensagem[0] = '\0';
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
                    inicioRanking = GetTime();
                }

                if (CheckCollisionPointRec(mouse, (Rectangle){150, 640, 300, 75})) {
                    ToggleBorderlessWindowed();
                }
            }

            comecarDesenho();

            ClearBackground(RAYWHITE);

            desenharMenu();

            terminarDesenho();

            continue;
        }

        if (telaAtual == TELA_SEQUENCIA) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                telaAtual = TELA_MENU;
                continue;
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                CheckCollisionPointRec(GetMousePosition(), (Rectangle){200, 730, 200, 50})) {
                telaAtual = TELA_MENU;
                continue;
            }

            comecarDesenho();

            ClearBackground((Color){240, 240, 235, 255});

            desenharSequencia(ranking);

            terminarDesenho();

            continue;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {

            telaAtual = TELA_MENU;
            entrada[0] = '\0';
            confeteAtivo = 0;

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

                    if (!sortearPalavraTxt(candidata)) {
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
                    confeteAtivo = 0;
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

                char clicada = teclaClicada();

                if (clicada >= 'A' && clicada <= 'Z' &&
                    strlen(entrada) < QUARTETO_TAMANHO) {

                    mensagem[0] = '\0';

                    int tamanho = (int)strlen(entrada);

                    entrada[tamanho] = clicada;
                    entrada[tamanho + 1] = '\0';
                }

                if (IsKeyPressed(KEY_BACKSPACE) || clicada == '\b') {

                    int tamanho = (int)strlen(entrada);

                    if (tamanho > 0) {
                        entrada[tamanho - 1] = '\0';
                    }
                }

                if ((IsKeyPressed(KEY_ENTER) || clicada == '\n') &&
                    strlen(entrada) == QUARTETO_TAMANHO) {
                    
                    int resultado = validarPalavraTxt(entrada);
                    
                    if (resultado != 1) { //animacao de quando a tentativa é inválida ou quando dá erro na consulta do dicionário
                        PlaySound(buzzer);
                        linhaTremendo = quarteto.quantidadeTentativas;
                        inicioTremor = GetTime();
                    }

                    if (resultado == 1) {
                        PlaySound(resp_ok);
                        enviarTentativaQuarteto(&quarteto, entrada);
                        inicioRevelacao = GetTime();
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

            comecarDesenho();

            ClearBackground(RAYWHITE);

            textoCentralizado("QUARTETO", 25, 35, DARKGRAY);

            desenharQuarteto(&quarteto, entrada, linhaTremendo, deslocamentoX);

            int teclado[26];

            limparTeclado(teclado);

            int linhasProntas = quarteto.quantidadeTentativas;

            if (!revelacaoAcabou()) {
                linhasProntas--;
            }

            for (int tab = 0; tab < QUARTETO_QUANTIDADE_PALAVRAS; tab++) {
                for (int i = 0; i < linhasProntas; i++) {
                    if (!quarteto.acertou[tab] || i < quarteto.tentativaAcerto[tab]) {
                        marcarTeclado(teclado, quarteto.tentativas[i], quarteto.cores[tab][i]);
                    }
                }
            }

            desenharTeclado(teclado);

            if (quarteto.terminou) {

                if (revelacaoAcabou()) {

                    if (quarteto.venceu && !confeteAtivo) {
                        iniciarConfete();
                    }

                    desenharResultadoQuarteto(&quarteto, deslocamento_resultadoY);
                }

            } else if (mensagem[0] != '\0') {

                textoCentralizado(mensagem, 665, 18, RED);
            }

            atualizarConfete();
            desenharConfete();

            terminarDesenho();

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

                char clicada = teclaClicada();

                if (clicada >= 'A' && clicada <= 'Z' &&
                    strlen(entrada) < DUETO_TAMANHO) {

                    mensagem[0] = '\0';

                    int tamanho = (int)strlen(entrada);

                    entrada[tamanho] = clicada;
                    entrada[tamanho + 1] = '\0';
                }
                if (IsKeyPressed(KEY_BACKSPACE) || clicada == '\b') {

                    int tamanho = (int)strlen(entrada);

                    if (tamanho > 0) {
                        entrada[tamanho - 1] = '\0';
                    }
                }

                if ((IsKeyPressed(KEY_ENTER) || clicada == '\n') &&
                    strlen(entrada) == DUETO_TAMANHO) {
                    
                    int resultadoValidacao = validarPalavraTxt(entrada);
                    
                    if (resultadoValidacao != 1) { //animacao de quando a tentativa é inválida ou quando dá erro na consulta do dicionário
                        PlaySound(buzzer);
                        linhaTremendo = dueto.quantidadeTentativas;
                        inicioTremor = GetTime();
                    }

                    if (resultadoValidacao == 1) {
                        PlaySound(resp_ok);
                        enviarTentativaDueto(&dueto, entrada);
                        inicioRevelacao = GetTime();
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

                if (sortearPalavraTxt(nova1)) {
                    for (int tentativa = 0; tentativa < 10; tentativa++) {
                        if (sortearPalavraTxt(nova2) &&
                            strcmp(nova1, nova2) != 0) {

                            iniciarDueto(&dueto, nova1, nova2);
                            inicioAnimResultado = -1.0;
                            confeteAtivo = 0;
                            entrada[0] = '\0';

                            break;
                        }
                    }
                }
            }

            comecarDesenho();

            ClearBackground(RAYWHITE);

            textoCentralizado("DUETO", 35, 40, DARKGRAY);

            desenharDueto(&dueto, entrada, linhaTremendo, deslocamentoX);

            int teclado[26];

            limparTeclado(teclado);

            int linhasProntas = dueto.quantidadeTentativas;

            if (!revelacaoAcabou()) {
                linhasProntas--;
            }

            for (int tab = 0; tab < DUETO_QUANTIDADE_PALAVRAS; tab++) {
                for (int i = 0; i < linhasProntas; i++) {
                    if (!dueto.acertou[tab] || i < dueto.tentativaAcerto[tab]) {
                        marcarTeclado(teclado, dueto.tentativas[i], dueto.cores[tab][i]);
                    }
                }
            }

            desenharTeclado(teclado);

            if (dueto.terminou) {

                if (revelacaoAcabou()) {

                    if (dueto.venceu && !confeteAtivo) {
                        iniciarConfete();
                    }

                    desenharResultadoDueto(&dueto, deslocamento_resultadoY);
                }

            } else if (mensagem[0] != '\0') {

                textoCentralizado(mensagem, 610, 18, RED);

            } else {

                textoCentralizado("Digite 5 letras e pressione enter", 610, 20, DARKGRAY);
            }

            atualizarConfete();
            desenharConfete();

            terminarDesenho();

            continue;
        }

        if (!jogo.terminou) {
            if (IsKeyPressed(KEY_LEFT)) {
                if (cursorTermo > 0) {
                cursorTermo--;
            }
        }

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_SPACE)) {
            if (cursorTermo < TAMANHO - 1) {
                cursorTermo++;
            }
        }

        int tecla = GetCharPressed();
        while (tecla > 0) {

            if (tecla >= 'a' && tecla <= 'z') {
                tecla = toupper(tecla);
            }

            if (tecla >= 'A' && tecla <= 'Z') {

                    mensagem[0] = '\0';

                    entrada[cursorTermo] = (char)tecla;

                    if (cursorTermo < TAMANHO - 1) {
                        cursorTermo++;
                    }
                }

                tecla = GetCharPressed();
            }

            char clicada = teclaClicada();
            if (clicada >= 'A' && clicada <= 'Z') {

                mensagem[0] = '\0';

                entrada[cursorTermo] = clicada;

                if (cursorTermo < TAMANHO - 1) {
                    cursorTermo++;
                }
            }

            if (IsKeyPressed(KEY_BACKSPACE) || clicada == '\b') {

                if (entrada[cursorTermo] == ' ' && cursorTermo > 0) {
                    cursorTermo--;
                }

                entrada[cursorTermo] = ' ';
            }

            int palavraCompleta = 1;

            for (int i = 0; i < TAMANHO; i++) {
                if (entrada[i] < 'A' || entrada[i] > 'Z') {
                    palavraCompleta = 0;
                    break;
                }
            }

            if ((IsKeyPressed(KEY_ENTER) || clicada == '\n') &&
                palavraCompleta) {
                
                
                int resultadoValidacao = validarPalavraTxt(entrada);

                if (resultadoValidacao != 1) { //animacao de quando a tentativa é inválida ou quando dá erro na consulta do dicionário
                    PlaySound(buzzer);
                    linhaTremendo = jogo.quantidadeTentativas;
                    inicioTremor = GetTime();
                }

                if (resultadoValidacao == 1) {
                    PlaySound(resp_ok);
                    enviarTentativa(&jogo, entrada);
                    inicioRevelacao = GetTime();
                    mensagem[0] = '\0';

                } else if (resultadoValidacao == 0) {

                    snprintf(mensagem, sizeof(mensagem),
                             "palavra não encontrada...");

                } else {

                    snprintf(mensagem, sizeof(mensagem),
                             "não foi possível consultar o dicionário");
                }

                for (int i = 0; i < TAMANHO; i++) {
                    entrada[i] = ' ';
                }

                entrada[TAMANHO] = '\0';
                cursorTermo = 0;
            }

        } else if (!registroSalvo) {

            adicionarResultado(&ranking, "Jogador",
                               jogo.quantidadeTentativas, jogo.venceu);

            salvarResultado("ranking.txt", "Jogador",
                            jogo.quantidadeTentativas, jogo.venceu);

            registroSalvo = 1;
        }

        if (jogo.terminou && IsKeyPressed(KEY_R)) {

            if (sortearPalavraTxt(palavra)) {
                iniciarJogo(&jogo, palavra);

                entrada[0] = '\0';

                // variaveis para animacoes:
                inicioAnimResultado = -1.0;
                confeteAtivo = 0;
                inicioTremor = -1.0;
                linhaTremendo = -1;

                //

                registroSalvo = 0;

            } else {

                printf("Erro ao obter nova palavra. Tente novamente.\n");
            }
        }

        comecarDesenho();

        ClearBackground(RAYWHITE);

        textoCentralizado("CERMO", 40, 40, DARKGRAY);

        desenharJogo(&jogo, entrada, linhaTremendo, deslocamentoX, cursorTermo);

        int teclado[26];

        limparTeclado(teclado);

        int linhasProntas = jogo.quantidadeTentativas;

        if (!revelacaoAcabou()) {
            linhasProntas--;
        }

        for (int i = 0; i < linhasProntas; i++) {
            marcarTeclado(teclado, jogo.tentativas[i], jogo.cores[i]);
        }

        desenharTeclado(teclado);

        if (jogo.terminou) {

            if (revelacaoAcabou()) {

                if (jogo.venceu && !confeteAtivo) {
                    iniciarConfete();
                }

                desenharResultadoTermo(&jogo, deslocamento_resultadoY);
            }

        } else if (mensagem[0] != '\0') {

            textoCentralizado(mensagem, 610, 18, RED);

        } else {

            textoCentralizado("Digite 5 letras e pressione enter", 610, 20, DARKGRAY);
        }

        atualizarConfete();
        desenharConfete();

        terminarDesenho();
    }

    
    UnloadSound(buzzer); 
    UnloadSound(resp_ok);
    
    CloseAudioDevice();
    UnloadRenderTexture(tela);
    CloseWindow();
    
    liberarRanking(ranking);

    liberarLista(&listaPalavras);

    return 0;
}