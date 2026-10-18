#include <string.h>
#include "jogo.h"

void iniciarJogo(Jogo *jogo, char palavra[]) {
    strcpy(jogo->palavra, palavra);

    jogo->quantidadeTentativas = 0;
    jogo->venceu = 0;
    jogo->terminou = 0;

    for (int i = 0; i < MAX_TENTATIVAS; i++) {
        jogo->tentativas[i][0] = '\0';

        for (int j = 0; j < TAMANHO; j++) {
            jogo->cores[i][j] = CINZA;
        }
    }
}

int enviarTentativa(Jogo *jogo, char tentativa[]) {
    if (jogo->terminou ||
        strlen(tentativa) != TAMANHO ||
        jogo->quantidadeTentativas >= MAX_TENTATIVAS) {
        return 0;
    }

    int linha = jogo->quantidadeTentativas;
    int usadas[TAMANHO] = {0};

    strcpy(jogo->tentativas[linha], tentativa);

    for (int i = 0; i < TAMANHO; i++) {
        if (tentativa[i] == jogo->palavra[i]) {
            jogo->cores[linha][i] = VERDE;
            usadas[i] = 1;
        }
    }

    for (int i = 0; i < TAMANHO; i++) {
        if (jogo->cores[linha][i] == VERDE) {
            continue;
        }

        for (int j = 0; j < TAMANHO; j++) {
            if (!usadas[j] && tentativa[i] == jogo->palavra[j]) {
                jogo->cores[linha][i] = AMARELO;
                usadas[j] = 1;
                break;
            }
        }
    }

    jogo->quantidadeTentativas++;

    if (strcmp(tentativa, jogo->palavra) == 0) {
        jogo->venceu = 1;
        jogo->terminou = 1;
    } else if (jogo->quantidadeTentativas == MAX_TENTATIVAS) {
        jogo->terminou = 1;
    }

    return 1;
}
