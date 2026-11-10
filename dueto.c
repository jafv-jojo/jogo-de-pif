
#include <stdio.h>
#include <string.h>
#include "dueto.h"

void iniciarDueto(Dueto *dueto, const char palavra1[], const char palavra2[]) {
    memset(dueto, 0, sizeof(Dueto));

    strcpy(dueto->palavras[0], palavra1);
    strcpy(dueto->palavras[1], palavra2);
}

int enviarTentativaDueto(Dueto *dueto, const char tentativa[]) {
    if (dueto->terminou || strlen(tentativa) != DUETO_TAMANHO ||
        dueto->quantidadeTentativas >= DUETO_MAX_TENTATIVAS) {
        return 0;
    }

    int linha = dueto->quantidadeTentativas;
    strcpy(dueto->tentativas[linha], tentativa);

    for (int tabuleiro = 0; tabuleiro < DUETO_QUANTIDADE_PALAVRAS; tabuleiro++) {
        if (dueto->acertou[tabuleiro]) {
            continue;
        }

        int usadas[DUETO_TAMANHO] = {0};
        const char *palavra = dueto->palavras[tabuleiro];

        for (int i = 0; i < DUETO_TAMANHO; i++) {
            if (tentativa[i] == palavra[i]) {
                dueto->cores[tabuleiro][linha][i] = 2;
                usadas[i] = 1;
            }
        }

        for (int i = 0; i < DUETO_TAMANHO; i++) {
            if (dueto->cores[tabuleiro][linha][i] == 2) {
                continue;
            }

            for (int j = 0; j < DUETO_TAMANHO; j++) {
                if (!usadas[j] && tentativa[i] == palavra[j]) {
                    dueto->cores[tabuleiro][linha][i] = 1;
                    usadas[j] = 1;
                    break;
                }
            }
        }

      if (strcmp(tentativa, palavra) == 0) {
        dueto->acertou[tabuleiro] = 1;
        dueto->tentativaAcerto[tabuleiro] = linha + 1;
        }
    }

    dueto->quantidadeTentativas++;

    if (dueto->acertou[0] && dueto->acertou[1]) {
        dueto->venceu = 1;
        dueto->terminou = 1;
    } else if (dueto->quantidadeTentativas == DUETO_MAX_TENTATIVAS) {
        dueto->terminou = 1;
    }

    return 1;
}
